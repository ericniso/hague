#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include "utils/initializer.h"
#include "klib/kseq.h"
#include "io/reader.h"
#include "cmdline/cmdline.h"
#include "graph/hgraph.h"

typedef struct gengetopt_args_info ggo_args;

uint8_t
main(uint64_t argc, char** argv)
{

    uint8_t result_code = EXIT_SUCCESS;

    ggo_args ai;
    assert(cmdline_parser(argc, argv, &ai) == 0);

    assert(ai.k_mer_length_arg > 1 && "k-mer length must be greater than 1");

    gzFile fp;
    kseq_t* seq = read_fasta(ai.filename_arg, &fp);

    hgraph* g = hgraph_create_de_bruijn_graph(seq, ai.k_mer_length_arg);

#ifdef DEBUG
    printf("Vertices: %d\nEdges: %d\n", hgraph_vertex_count(g), hgraph_edge_count(g));
#endif

    hgraph_compute_eulerian_path_properties(g);

    if (hgraph_has_eulerian_properties(g))
    {
#ifdef DEBUG
        hgraph_vertex* s = hgraph_eulerian_walk_start(g);
        hgraph_vertex* e = hgraph_eulerian_walk_end(g);

        if (hgraph_has_eulerian_cycle(g))
            printf("Eulerian cycle, picking arbitrary starting vertex\n");

        if (hgraph_has_eulerian_path(g))
            printf("Eulerian path\n");

        printf("Start: %s\nEnd: %s\n", s->key, e->key);

        printf("Path result: ");
#endif
        char* superstring = hgraph_compute_eulerian_walk(g);
        printf("%s", superstring);
        free(superstring);

#ifdef DEBUG
        printf("\n");
#endif
    }
    else
    {
#ifdef DEBUG
        printf("Not an eulerian path");
#endif
        result_code = EXIT_FAILURE;    
    }

    hgraph_destroy(g);
    kseq_destroy(seq);
    gzclose(fp);
    cmdline_parser_free(&ai);

    return result_code;
}
