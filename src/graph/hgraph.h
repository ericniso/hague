#ifndef HAGUE_GRAPH_H
#define HAGUE_GRAPH_H

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <string.h>

typedef struct hgraph hgraph;

typedef struct hgraph_vertex hgraph_vertex;

typedef struct hgraph_edge hgraph_edge;

struct hgraph
{
    int v; /* Number of vertices */
    int e; /* Number of edges */
    int v_balanced; /* Number of balanced vertices */
    int v_semi_balanced; /* Number of semi-balanced vertices */
    int v_generic; /* Number of vertices with different in/out edges */
    hgraph_vertex* w_start; /* Starting vertex of Eulerian path (if exists) */
    hgraph_vertex* w_end; /* Ending vertex of Eulerian path (if exists) */
    hgraph_vertex** vertices; /* Map of vertices */
};

struct hgraph_vertex
{
    char* key; /* Node identifier */
    int indegree; /* Indegree */
    int outdegree; /* Outdegree */ 
    struct hgraph_edge** neighbours; /* Connected edges */
};

struct hgraph_edge
{
    bool visited; /* Needed for Eulerian walk algorithm */
    char* end; /* Ending node key */
};

hgraph*
hgraph_create();

int
hgraph_vertex_count(hgraph*);

int
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
hgraph_compute__eulerian_path_properties(hgraph*);

bool
hgraph_has_eulerian_path(hgraph*);

bool
hgraph_has_eulerian_cycle(hgraph*);

bool
hgraph_has_eulerian_properties(hgraph*);

#endif
