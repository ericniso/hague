#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <stdint.h>
#include "klib/kseq.h"
#include "io/reader.h"
#include "cmdline/cmdline.h"
#include "graph/hgraph.h"

typedef struct gengetopt_args_info ggo_args;

hgraph*
create_de_bruijn_graph(kseq_t*, uint64_t);

char*
strupr(char*);

uint64_t
main(uint64_t argc, char** argv)
{
    ggo_args ai;
    assert(cmdline_parser(argc, argv, &ai) == 0);

    gzFile fp;
    kseq_t* seq = read_fasta(ai.filename_arg, &fp);

    hgraph* g = create_de_bruijn_graph(seq, ai.k_mer_length_arg);

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
        printf("%s", hgraph_compute_eulerian_walk(g));

#ifdef DEBUG
        printf("\n");
#endif
    }
    else
    {
#ifdef DEBUG
        printf("Not an eulerian path");
#endif    
    }

    hgraph_destroy(g);
    kseq_destroy(seq);
    gzclose(fp);
    cmdline_parser_free(&ai);

    return 0;
}

hgraph*
create_de_bruijn_graph(kseq_t* seq, uint64_t k)
{
    hgraph* g = hgraph_create();

    while ((kseq_read(seq)) >= 0)
    {
        char* s = seq->seq.s;

        assert(strlen(s) >= k && "Sequence length must be equal to or greater than k-mer length");

        for (uint64_t i = 0; i < strlen(s) - k + 1; i++)
        {
            char* kmer = malloc(k * sizeof(char) + 1);
            strncpy(kmer, &s[i], k);
            kmer[k] = '\0';
            kmer = strupr(kmer);

            char* lk = malloc((k - 1) * sizeof(char) + 1);
            char* rk = malloc((k - 1) * sizeof(char) + 1);
            strncpy(lk, kmer, (k - 1));
            lk[k - 1] = '\0';
            strncpy(rk, &kmer[1], (k - 1));
            rk[k - 1] = '\0';
            
            hgraph_add_edge(g, lk, rk);
        }
    }

    return g;
}

char*
strupr(char* s_l)
{
    char* s_u = malloc(strlen(s_l) * sizeof(char) + 1);
    uint64_t i = 0;
    while(s_l[i] != '\0')
    {   
        char c;

        if (s_l[i] >= 'a' && s_l[i] <= 'z')
            c = toupper(s_l[i]);
        else
            c = s_l[i];

        s_u[i] = c;
        i++;
    }
    
    s_u[strlen(s_l) + 1] = '\0';
    return s_u;
}
