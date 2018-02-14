#include "hgraph.h"

/**
 *  Detect if the graph g has been initialized
 */
static inline void
assert_graph_init(hgraph* g)
{
    assert(g != NULL && "Graph is not initialized");
}

/**
 *  Detect if eulerian properties of graph g have been computed
 */
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

/**
 *  @param g An initialized hague graph
 *  @return Number of vertices in g
 */
uint64_t
hgraph_vertex_count(hgraph* g)
{
    assert_graph_init(g);

    return g->v;
}

/**
 * @param g An initialized hague graph
 * @return Number of edges in g
 */
uint64_t
hgraph_edge_count(hgraph* g)
{
    assert_graph_init(g);

    return g->e;
}

/**
 *  @param g An initialized hague graph
 *  @param key The label of the vertex
 *  @return An hague vertex if g has a vertex with label "key", NULL otherwise
 */
hgraph_vertex*
hgraph_get_vertex(hgraph* g, char* key)
{
    assert_graph_init(g);

    hgraph_vertex* v = NULL;

    HASH_FIND_STR(g->vertices, key, v);

    return v;
}

/**
 * @param g An initialized hague graph
 * @param key The label of the vertex
 * @return An hague vertex if g doesn't have a vertex with label "key", NULL otherwise
 */
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

/**
 *  Create edge from start to end in graph g
 */
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

/**
 * Destroy graph g
 */
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

/**
 *  Return the first vertex of the eulerian walk
 */
hgraph_vertex*
hgraph_eulerian_walk_start(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->w_start;
}

/**
 *  Return the last vertex of the eulerian walk
 */
hgraph_vertex*
hgraph_eulerian_walk_end(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->w_end;
}

/**
 *  Compute the properties of the eulerian walk for graph g
 */
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

/**
 *  Return true if and only if graph g has eulerian path
 */
bool
hgraph_has_eulerian_path(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->v_generic == 0 && g->v_semi_balanced == 2;
}

/**
 *  Return true if and only if graph g has eulerian cycle
 */
bool
hgraph_has_eulerian_cycle(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->v_generic == 0 && g->v_semi_balanced == 0;
}

/**
 *  Return true if and only if eulerian properties have been computed on graph g
 */
bool
hgraph_has_eulerian_properties(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return hgraph_has_eulerian_path(g) || hgraph_has_eulerian_cycle(g);
}

/**
 *  Compute the eulerian walk on graph g
 */
char*
hgraph_compute_eulerian_walk(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    hgraph_vertex* start = hgraph_eulerian_walk_start(g);
    hgraph_vertex* end = hgraph_eulerian_walk_end(g);
    hgraph_vertex* next = start;

    uint64_t kmer = strlen(start->key) + 1;
    uint64_t result_length = g->e + kmer - 1;

    char* result = malloc(result_length * sizeof(char) + 1);

    for (uint64_t i = 0; i < result_length - kmer + 1; i++)
    {   
        uint64_t j = next->next_neighbour;
        if (j < next->outdegree)
        {
            hgraph_edge* edge = next->neighbours[j];
            next->next_neighbour++;

            // haven't reached the last edge yet
            if (i < result_length - kmer)
            {
                next = hgraph_get_vertex(g, edge->next);

                result[i] = edge->label[0];
            }
            else
            {
                strncpy(&result[i], edge->label, strlen(edge->label));
            }
        }
    }

    result[result_length] = '\0';

    return result;
}

/**
 *  Create De Bruijn graph from a FASTA sequence using substring of the sequence of length k(k-mer)
 */
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

/**
 *  Save graph to file using Gephi "Edges table" notation
 */
void
hgraph_export_to_file(hgraph* g, char* filename)
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

/**
 *  Print graph to console using Gephi "Edges table" notation
 */
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