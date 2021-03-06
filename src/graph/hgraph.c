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

    uint64_t count_vertices_by_type = (g->count_balanced_vertices + g->count_semi_balanced_vertices
                                                                  + g->count_generic_vertices);
    assert(count_vertices_by_type == g->count_vertices &&
                   "Eulerian properties not computed, try calling hgraph_compute__eulerian_path_properties(hgraph)");

}

hgraph*
hgraph_create()
{
    hgraph* g = malloc(sizeof(hgraph));
    g->count_vertices = 0;
    g->count_edges = 0;
    g->count_semi_balanced_vertices = 0;
    g->count_balanced_vertices = 0;
    g->count_generic_vertices = 0;
    g->walk_start_vertex = NULL;
    g->walk_end_vertex = NULL;
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

    return g->count_vertices;
}

/**
 * @param g An initialized hague graph
 * @return Number of edges in g
 */
uint64_t
hgraph_edge_count(hgraph* g)
{
    assert_graph_init(g);

    return g->count_edges;
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

        g->count_vertices++;
        HASH_ADD_STR(g->vertices, key, v);
    }    

    return v;
}

/**
 * @param g An initialized hague graph
 * @param start Label of starting node
 * @param end Label of ending node
 * @return The created hague edge
 *
 * The label of the edge is obtained concatenating the label of the starting node and the last
 * character of the label of the ending node
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

    g->count_edges++;

    return e;
}

/**
 * @param g An initialized hague graph
 *
 * Remove the graph from memory
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
 * @param g An initialized hague graph
 * @return The eulerian walk starting vertex if exists, NULL otherwise
 */
hgraph_vertex*
hgraph_eulerian_walk_start(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->walk_start_vertex;
}

/**
 * @param g An initialized hague graph
 * @return The eulerian walk ending vertex if exists, NULL otherwise
 */
hgraph_vertex*
hgraph_eulerian_walk_end(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return g->walk_end_vertex;
}

/**
 * @param g An initialized hague graph
 *
 * Set the starting and ending node of the eulerian walk(if exists) and detect if the graph is eulerian, semi-eularian
 * or a generic graph
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
            g->count_balanced_vertices++;
        }
        else if (abs(v->indegree - v->outdegree) == 1)
        {
            g->count_semi_balanced_vertices++;

            if (v->indegree == v->outdegree + 1)
            {
                g->walk_end_vertex = v;
            }

            if (v->outdegree == v->indegree + 1)
            {
                g->walk_start_vertex = v;
            }
        }
        else
        {
            g->count_generic_vertices++;
        }
    }

    if (hgraph_has_eulerian_cycle(g))
    {
        g->walk_start_vertex = first;
        g->walk_end_vertex = g->walk_start_vertex;
    }
}

/**
 * @param g An initialized hague graph
 * @return True, if g has eulerian path, false otherwise
 */
bool
hgraph_has_eulerian_path(hgraph* g)
{
    assert_eulerian_properties_computed(g);
    bool is_semi_eulerian = (g->count_semi_balanced_vertices == 2) && (g->count_generic_vertices == 0);

    return is_semi_eulerian;
}

/**
 * @param g An initialized hague graph
 * @return True, if g has eulerian cycle, false otherwise
 */
bool
hgraph_has_eulerian_cycle(hgraph* g)
{
    assert_eulerian_properties_computed(g);
    bool is_eulerian = (g->count_generic_vertices == 0) && (g->count_semi_balanced_vertices == 0);
    return is_eulerian;
}

/**
 * @param g An initialized hague graph
 * @return True, if eulerian properties have already been computed on g, false otherwise
 */
bool
hgraph_has_eulerian_properties(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    return hgraph_has_eulerian_path(g) || hgraph_has_eulerian_cycle(g);
}

/**
 * @param g An initialized hague graph
 * @return A string containing the concatenation of edge labels, from eulerian walk starting node to ending node
 *
 * Eulerian properties must have been already computed on g
 */
char*
hgraph_compute_eulerian_walk(hgraph* g)
{
    assert_eulerian_properties_computed(g);

    hgraph_vertex* start = hgraph_eulerian_walk_start(g);
    hgraph_vertex* end = hgraph_eulerian_walk_end(g);
    hgraph_vertex* next = start;

    uint64_t kmer_length = strlen(start->key) + 1;
    uint64_t result_length = g->count_edges + kmer_length - 1;

    char* result = malloc(result_length * sizeof(char) + 1);

    for (uint64_t i = 0; i < result_length - kmer_length + 1; i++)
    {   
        uint64_t j = next->next_neighbour;
        if (j < next->outdegree)
        {
            hgraph_edge* edge = next->neighbours[j];
            next->next_neighbour++;

            // haven't reached the last edge yet
            if (i < result_length - kmer_length)
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
 * @param seq A FASTA sequence parsed using kseq library
 * @param k The length of the k-mer
 * @return An empty hague graph if seq is not valid or an hague graph representing a De Bruijn graph otherwise
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
 * @param g An initialized hague graph
 * @param filename Name of the outpur file
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
 * @param g An initialized hague graph
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