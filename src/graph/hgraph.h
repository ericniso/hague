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

#endif
