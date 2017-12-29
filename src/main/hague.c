#include "../../libs/klib/kseq.h"
#include "../io/io.h"
#include "../cmdline/cmdline.h"

typedef struct gengetopt_args_info ggo_args;

int
main(int argc, char** argv)
{
    ggo_args ai;
    assert(cmdline_parser(argc, argv, &ai) == 0);

    return 0;

}
