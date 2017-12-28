#include "hgraph.h"

static inline void
assert_graph_init(hgraph* g)
{
    assert(g != NULL && "Graph is not initialized");
}

hgraph*
hgraph_create()
{
    hgraph* g = malloc(sizeof(hgraph));
    g->v = 0;
    g->e = 0;
    g->vertices = malloc(g->v * sizeof(hgraph_vertex*));

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
        v->key = malloc(sizeof(char));
        strcpy(v->key, key);
        v->neighbours = malloc(0 * sizeof(hgraph_edge*));

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

    v_s->neighbours = realloc(v_s->neighbours, sizeof(v_s->outdegree * sizeof(hgraph_edge*)));

    hgraph_edge* e = malloc(sizeof(hgraph_edge));
    e->visited = false;
    e->end = malloc(sizeof(char));
    strcpy(e->end, end);

    v_s->neighbours[v_s->outdegree - 1] = e;

    g->e++;
}
