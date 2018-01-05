#ifndef HAGUE_GRAPH_H
#define HAGUE_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>

typedef struct hgraph hgraph;

typedef struct hgraph_vertex hgraph_vertex;

typedef struct hgraph_edge hgraph_edge;

struct hgraph
{
    uint64_t v; /* Number of vertices */
    uint64_t e; /* Number of edges */
    uint64_t v_balanced; /* Number of balanced vertices */
    uint64_t v_semi_balanced; /* Number of semi-balanced vertices */
    uint64_t v_generic; /* Number of vertices with different in/out edges */
    hgraph_vertex* w_start; /* Starting vertex of Eulerian path (if exists) */
    hgraph_vertex* w_end; /* Ending vertex of Eulerian path (if exists) */
    hgraph_vertex** vertices; /* Map of vertices */
};

struct hgraph_vertex
{
    char* key; /* Node identifier */
    uint64_t indegree; /* Indegree */
    uint64_t outdegree; /* Outdegree */ 
    struct hgraph_edge** neighbours; /* Connected edges */
};

struct hgraph_edge
{
    bool visited; /* Needed for Eulerian walk algorithm */
    char* end; /* Ending node key */
};

hgraph*
hgraph_create();

uint64_t
hgraph_vertex_count(hgraph*);

uint64_t
hgraph_edge_count(hgraph*);

hgraph_vertex*
hgraph_get_vertex(hgraph*, char*);

void
hgraph_add_vertex(hgraph*, char*);

void
hgraph_add_edge(hgraph*, char*, char*);

void
hgraph_destroy(hgraph*);

hgraph_vertex*
hgraph_eulerian_walk_start(hgraph*);

hgraph_vertex*
hgraph_eulerian_walk_end(hgraph*);

void
hgraph_compute_eulerian_path_properties(hgraph*);

bool
hgraph_has_eulerian_path(hgraph*);

bool
hgraph_has_eulerian_cycle(hgraph*);

bool
hgraph_has_eulerian_properties(hgraph*);

char*
hgraph_compute_eulerian_walk(hgraph*);

hgraph_vertex*
__hgraph_eulerian_walk_next_vertex(hgraph* g, hgraph_vertex* src);

#endif
