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
    g->w_start = NULL;
    g->w_end = NULL;
    g->vertices = NULL;

    return g;
}

uint64_t
hgraph_vertex_count(hgraph* g)
{
    assert_graph_init(g);

    return g->v;
}

uint64_t
hgraph_edge_count(hgraph* g)
{
    assert_graph_init(g);

    return g->e;
}

hgraph_vertex*
hgraph_get_vertex(hgraph* g, char* key)
{
    assert_graph_init(g);

    hgraph_vertex* v = NULL;

    HASH_FIND_STR(g->vertices, key, v);

    return v;
}

hgraph_vertex*
hgraph_add_vertex(hgraph* g, char* key)
{
    assert_graph_init(g);

    hgraph_vertex* v = hgraph_get_vertex(g, key);
    if (v == NULL)
    {
        v = malloc(sizeof(hgraph_vertex));
        v->indegree = 0;
        v->outdegree = 0;
        v->next_neighbour = 0;
        v->key = malloc(strlen(key) * sizeof(char) + 1);
        strcpy(v->key, key);
        v->neighbours = NULL;

        g->v++;
        HASH_ADD_STR(g->vertices, key, v);
    }    

    return v;
}

hgraph_edge*
hgraph_add_edge(hgraph* g, char* start, char* end)
{
    assert_graph_init(g);

    hgraph_vertex* v_s = hgraph_add_vertex(g, start);
    v_s->outdegree++;
    
    hgraph_vertex* v_e = hgraph_add_vertex(g, end);
    v_e->indegree++;

    v_s->neighbours = realloc(v_s->neighbours, v_s->outdegree * sizeof(hgraph_edge*));

    hgraph_edge* e = malloc(sizeof(hgraph_edge));
    uint64_t llength = strlen(start) + 1;
    e->label = malloc(llength * sizeof(char) + 1);
    e->next = malloc(strlen(end) * sizeof(char) + 1);
    strcpy(e->next, end);
    strncpy(e->label, start, strlen(start)); // Copy first (k-1)-mer chars
    strncpy(&(e->label[llength - 1]), &(end[strlen(end) - 1]), 1); // copy last char
    e->label[llength] = '\0';

    v_s->neighbours[v_s->outdegree - 1] = e;

    g->e++;

    return e;
}

void
hgraph_destroy(hgraph* g)
{
    assert_graph_init(g);

    hgraph_vertex* v = NULL;
    hgraph_vertex* tmp = NULL;

    HASH_ITER(hh, g->vertices, v, tmp)
    {

        HASH_DEL(g->vertices, v);

        for (uint64_t j = 0; j < v->outdegree; j++)
        {
            hgraph_edge* e = v->neighbours[j];
            free(e->label);
            free(e->next);
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

    hgraph_vertex* v = NULL;
    hgraph_vertex* first = NULL;
    hgraph_vertex* tmp = NULL;

    HASH_ITER(hh, g->vertices, v, tmp) {
        
        if (first == NULL)
        {
            first = v;
        }

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
        g->w_start = first;
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

    hgraph_vertex* start = hgraph_eulerian_walk_start(g);
    hgraph_vertex* end = hgraph_eulerian_walk_end(g);

    uint64_t result_length = g->e + strlen(end->key);
    char* result = malloc(result_length * sizeof(char) + 1);

    result[0] = start->key[0];

    hgraph_vertex* next = start;

    uint64_t i;
    for (i = 1; i < result_length - strlen(end->key); i++)
    {   
        uint64_t j = next->next_neighbour;
        if (j < next->outdegree)
        {
            hgraph_edge* e = next->neighbours[j];
            next->next_neighbour++;
            next = hgraph_get_vertex(g, e->next);
        }

        assert(next != NULL && "Not an Eulerian path");
        result[i] = next->key[0];
    }

    uint64_t j = 0;
    for (i; i < result_length; i++)
    {
        result[i] = end->key[j];
        j++;
    }

    result[result_length] = '\0';

    return result;
}

hgraph*
hgraph_create_de_bruijn_graph(kseq_t* seq, uint64_t k)
{
    hgraph* g = hgraph_create();
    bool validfile = false;
    while ((kseq_read(seq)) >= 0)
    {
        validfile = true;
        char* s = seq->seq.s;

        assert(strlen(s) >= k && "Sequence length must be equal to or greater than k-mer length");

        for (uint64_t i = 0; i < strlen(s) - k + 1; i++)
        {
            char* kmer = malloc(k * sizeof(char) + 1);
            strncpy(kmer, &s[i], k);
            kmer[k] = '\0';

            char* lk = malloc((k - 1) * sizeof(char) + 1);
            char* rk = malloc((k - 1) * sizeof(char) + 1);
            strncpy(lk, kmer, (k - 1));
            lk[k - 1] = '\0';
            strncpy(rk, &kmer[1], (k - 1));
            rk[k - 1] = '\0';

            hgraph_add_edge(g, lk, rk);

            free(kmer);
            free(lk);
            free(rk);
        }
    }
    assert(validfile && "Invalid file content");

    return g;
}

void
hgraph_export_to_csv(hgraph* g, char* filename)
{
    assert_graph_init(g);
    hgraph_vertex* v = NULL;
    hgraph_vertex* tmp = NULL;
    FILE *f = fopen(filename, "w");
    fprintf(f, "Source, Target, Label\n");

    HASH_ITER(hh, g->vertices, v, tmp)
    {
        for(uint64_t i = 0; i < v->outdegree; i++)
        {
            hgraph_edge* edge = v->neighbours[i];
            fprintf(f, "%s, %s, %s\n", v->key, edge->next, edge->label);
        }
    }

    fclose(f);
}

void
hgraph_print_graph(hgraph* g)
{
    assert_graph_init(g);
    hgraph_vertex* v = NULL;
    hgraph_vertex* tmp = NULL;
    printf("Source, Target, Label\n");

    HASH_ITER(hh, g->vertices, v, tmp)
    {
        for(uint64_t i = 0; i < v->outdegree; i++)
        {
            hgraph_edge* edge = v->neighbours[i];
            printf("%s, %s, %s\n", v->key, edge->next, edge->label);
        }
    }
}