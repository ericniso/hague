#ifndef HAGUE_GRAPH_H
#define HAGUE_GRAPH_H

#include <stdbool.h>
#include <assert.h>
#include "../map/map.h"

typedef struct hgraph hgraph;

typedef struct hgraph_vertex hgraph_vertex;

typedef struct hgraph_edge hgraph_edge;

typedef map_t(hgraph_vertex*) hgraph_vertex_map;

struct hgraph
{
    int v; /* Number of vertices */
    int e; /* Number of edges */
    hgraph_vertex_map* vertices; /* Map of vertices */
};

struct hgraph_vertex
{
    char* key; /* Node identifier */
    struct hgraph_edge** neighbours; /* Connected edges */
};

struct hgraph_edge
{
    bool visited; /* Needed for Eulerian walk algorithm */
    char* end; /* Ending node key */
};

#endif
