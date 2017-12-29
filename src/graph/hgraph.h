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
    hgraph_vertex* walk_start; /* Eulerian walk starting node */
    hgraph_vertex* walk_end; /* Eulerian walk ending node */
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

void
__update_walk_start_end(hgraph*, hgraph_vertex*);

hgraph_vertex*
hgraph_eulerian_walk_start(hgraph*);

hgraph_vertex*
hgraph_eulerian_walk_end(hgraph*);

#endif
