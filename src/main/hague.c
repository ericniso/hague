#include <stdlib.h>
#include <stdio.h>
#include <zlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../../libs/klib/kseq.h"
#include "../io/reader.h"
#include "../cmdline/cmdline.h"

typedef struct gengetopt_args_info ggo_args;

void
create_de_bruijn_graph(kseq_t*, int);

int
main(int argc, char** argv)
{
    ggo_args ai;
    assert(cmdline_parser(argc, argv, &ai) == 0);

    return 0;
}

char*
strupr(char* s_l)
{
    char* s_u = malloc(strlen(s_l) * sizeof(char) + 1);
    int i = 0;
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
