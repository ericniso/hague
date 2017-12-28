#ifndef HAGUE_H
#define HAGUE_H

#include <assert.h>
#include <zlib.h>
#include "../../libs/klib/kseq.h"
#include "../cmdline/cmdline.h"

KSEQ_INIT(gzFile, gzread)

typedef struct gengetopt_args_info ggo_args;

void
read_fasta(char* filename, kseq_t* seq);

#endif
