#include "hgraph.h"

static inline void
assert_graph_init(hgraph* g)
{
    assert(g != NULL && "Graph is not initialized");
}

static inline void
assert_eulerian_properties_computed(hgraph* g)
{
    assert_graph_init(g);

    assert((g->v_balanced + g->v_semi_balanced + g->v_generic) == g->v
        && "Eulerian properties not computed, try calling hgraph_compute__eulerian_path_properties(hgraph)");

}

hgraph*
hgraph_create()
{
    hgraph* g = malloc(sizeof(hgraph));
    g->v = 0;
    g->e = 0;
    g->v_semi_balanced = 0;
    g->v_balanced = 0;
    g->v_generic = 0;
    g->vertices = NULL;

    return g;
}

int
hgraph_vertex_count(hgraph* g)
{
    assert_graph_init(g);

    return g->v;
}

int
hgraph_edge_count(hgraph* g)
{
    assert_graph_init(g);

    return g->e;
}

hgraph_vertex*
hgraph_get_vertex(hgraph* g, char* key)
{
    assert_graph_init(g);

    int i = 0;
    bool found = false;
    hgraph_vertex* v = NULL;

    while(!found && i < g->v)
    {
        hgraph_vertex* curr = g->vertices[i];
        if (strcmp(curr->key, key) == 0)
        {
            v = curr;
            found = true;
        }

        i++;
    }

    return v;
}

void
hgraph_add_vertex(hgraph* g, char* key)
{
    assert_graph_init(g);

    hgraph_vertex* v = hgraph_get_vertex(g, key);
    if (v == NULL)
    {
        v = malloc(sizeof(hgraph_vertex));
        v->indegree = 0;
        v->outdegree = 0;
        v->key = malloc(strlen(key) * sizeof(char) + 1);
        strcpy(v->key, key);
        v->neighbours = NULL;

        g->v++;
        g->vertices = realloc(g->vertices, g->v * sizeof(hgraph_vertex*));
        g->vertices[g->v - 1] = v;
    }    
}

void
hgraph_add_edge(hgraph* g, char* start, char* end)
{
    assert_graph_init(g);

    hgraph_add_vertex(g, start);
    hgraph_add_vertex(g, end);

    hgraph_vertex* v_e = hgraph_get_vertex(g, end);
    v_e->indegree++;

    hgraph_vertex* v_s = hgraph_get_vertex(g, start);
    v_s->outdegree++;

    v_s->neighbours = realloc(v_s->neighbours, v_s->outdegree * sizeof(hgraph_edge*));

    hgraph_edge* e = malloc(sizeof(hgraph_edge));
    e->visited = false;
    e->end = malloc(strlen(end) * sizeof(char) + 1);
    strcpy(e->end, end);

    v_s->neighbours[v_s->outdegree - 1] = e;

    g->e++;
}

void
hgraph_destroy(hgraph* g)
{
    assert_graph_init(g);

    for (int i = 0; i < g->v; i++)
    {
        hgraph_vertex* v = g->vertices[i];

        for (int j = 0; j < v->outdegree; j++)
        {
            hgraph_edge* e = v->neighbours[j];
            free(e->end);
            free(e);
        }

        free(v->key);
        free(v->neighbours);
        free(v);
    }

    free(g->vertices);
    free(g);
}

hgraph_vertex*
hgraph_eulerian_walk_start(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->w_start;
}

hgraph_vertex*
hgraph_eulerian_walk_end(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->w_end;
}

void
hgraph_compute_eulerian_path_properties(hgraph* g)
{
    assert_graph_init(g);

    for (int i = 0; i < g->v; i++)
    {
        hgraph_vertex* v = g->vertices[i];
        if (v->indegree == v->outdegree)
        {
            g->v_balanced++;
        }
        else if (abs(v->indegree - v->outdegree) == 1)
        {
            g->v_semi_balanced++;

            if (v->indegree == v->outdegree + 1)
            {
                g->w_end = v;
            }

            if (v->outdegree == v->indegree + 1)
            {
                g->w_start = v;
            }
        }
        else
        {
            g->v_generic++;
        }
    }

    if (hgraph_has_eulerian_cycle(g))
    {
        g->w_start = g->vertices[0];
        g->w_end = g->w_start;
    }
}

bool
hgraph_has_eulerian_path(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->v_generic == 0 && g->v_semi_balanced == 2;
}

bool
hgraph_has_eulerian_cycle(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->v_generic == 0 && g->v_semi_balanced == 0;
}

bool
hgraph_has_eulerian_properties(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return hgraph_has_eulerian_path(g) || hgraph_has_eulerian_cycle(g);
}

char*
hgraph_compute_eulerian_walk(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    int result_length = g->e + 1 + 1;
    char* result = malloc(result_length * sizeof(char) + 1);

    hgraph_vertex* start = hgraph_eulerian_walk_start(g);
    hgraph_vertex* end = hgraph_eulerian_walk_end(g);

    result[0] = start->key[0];

    hgraph_vertex* next = start;

    for (int i = 1; i < result_length - 1; i++)
    {   
        next = __hgraph_eulerian_walk_next_vertex(g, next);
        assert(next != NULL && "Not an Eulerian path");
        result[i] = next->key[0];
    }

    result[result_length - 1] = end->key[strlen(end->key) -1];
    result[result_length] = '\0';

    return result;
}

hgraph_vertex*
__hgraph_eulerian_walk_next_vertex(hgraph* g, hgraph_vertex* src)
{
    hgraph_vertex* v = NULL;
    
    int i = 0;
    while (v == NULL && i < src->outdegree)
    {
        hgraph_edge* e = src->neighbours[i];

        if (!e->visited)
        {
            e->visited = true;
            v = hgraph_get_vertex(g, e->end);
        }

        i++;
    }

    return v;
}
