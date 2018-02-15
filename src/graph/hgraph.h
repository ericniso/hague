#ifndef HAGUE_GRAPH_H
#define HAGUE_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "utils/initializer.h"
#include "klib/kseq.h"
#include "hash/uthash.h"

typedef struct hgraph hgraph;

typedef struct hgraph_vertex hgraph_vertex;

typedef struct hgraph_edge hgraph_edge;

/** @struct hgraph
    @brief A struct representing an "Hague Graph"

    An Hague Graph is a standard adjacency-list based graph enriched with fields useful
    for managing eulerian graphs.
*/
struct hgraph
{
    uint64_t v; /**<  Number of vertices */
    uint64_t e; /**<  Number of edges */
    uint64_t v_balanced; /**< Number of balanced vertices */
    uint64_t v_semi_balanced; /**< Number of semi-balanced vertices */
    uint64_t v_generic; /**< Number of vertices with different in/out edges */
    hgraph_vertex* w_start; /**< Starting vertex of Eulerian path (if exists) */
    hgraph_vertex* w_end; /**< Ending vertex of Eulerian path (if exists) */
    hgraph_vertex* vertices; /**< Map of vertices */
};

/** @struct hgraph_vertex
    @brief A struct representing an "Hague Graph" vertex

    An hague vertex has a key field which is also the vertex label(i.e the k-1-mer), two fields are used
    to store the degree of the vertex and a list of adjacency is used to mantain graph structure.
    The struct is also hashed to let perform operation using hash operators.
*/
struct hgraph_vertex
{
    char* key; /**< Node identifier */
    uint64_t indegree; /**< Indegree */
    uint64_t outdegree; /**< Outdegree */ 
    uint64_t next_neighbour; /**< Next neighbour index, needed for linear search */
    struct hgraph_edge** neighbours; /**< Connected edges */
    UT_hash_handle hh; /**< Make this struct hashable */
};

/** @struct hgraph_edge
    @brief A struct representing an "Hague Graph" edge

    An hague edge has a label field(i.e the k-mer) and a next field which is the label of
    the the ending vertex of the directed edge
*/
struct hgraph_edge
{
    char* label; /**< k-mer string */
    char* next; /**< Next node key */
};

/**
 * @brief Create hague graph
 *
 * @return initialized hague graph
 */
hgraph*
hgraph_create();

/**
 * @brief Return the number of vertex in an hague graph
*/
uint64_t
hgraph_vertex_count(hgraph*);

/**
 *
 * @brief Return the number of edges in an hague graph
 */
uint64_t
hgraph_edge_count(hgraph*);

/**
 *
 * @brief Return an hague vertex from a given hague graph
 */
hgraph_vertex*
hgraph_get_vertex(hgraph*, char*);

/**
 *
 * @brief Create a vertex in an hague graph
 */
hgraph_vertex*
hgraph_add_vertex(hgraph*, char*);

/**
 *
 * @brief Create an edge in an hague graph
 */
hgraph_edge*
hgraph_add_edge(hgraph*, char*, char*);

/**
 *
 * @brief Destroy graph g
 */
void
hgraph_destroy(hgraph*);

/**
 *
 * @brief Return the first vertex of the eulerian walk of an hague graph(if exists)
 */
hgraph_vertex*
hgraph_eulerian_walk_start(hgraph*);

/**
 *
 * @brief Return the last vertex of the eulerian walk of an hague graph(if exists)
 */
hgraph_vertex*
hgraph_eulerian_walk_end(hgraph*);

/**
 *
 * @brief Compute the properties of the eulerian walk for an hague graph
 */
void
hgraph_compute_eulerian_path_properties(hgraph*);

/**
 *
 * @brief Return true if and only if an hague graph has eulerian path
 */
bool
hgraph_has_eulerian_path(hgraph*);

/**
 *
 * @brief Return true if and only if an hague graph has eulerian cycle
 */
bool
hgraph_has_eulerian_cycle(hgraph*);

/**
 *
 * @brief Return true if and only if eulerian properties have already been computed on an hague graph
 */
bool
hgraph_has_eulerian_properties(hgraph*);

/**
 *
 * @brief Compute the eulerian walk on an hague graph
 */
char*
hgraph_compute_eulerian_walk(hgraph*);

/**
 *
 * @brief Create De Bruijn graph from a FASTA sequence
 */
hgraph*
hgraph_create_de_bruijn_graph(kseq_t*, uint64_t);

/**
 *
 * @brief Save graph to file using Gephi "Edges table" notation
 */
void
hgraph_export_to_file(hgraph*, char*);

/**
 *
 * @brief Print graph to console using Gephi "Edges table" notation
 */
void
hgraph_print_graph(hgraph*);

#endif
