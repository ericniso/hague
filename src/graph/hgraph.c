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

