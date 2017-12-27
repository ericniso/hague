#ifndef HAGUE_H
#define HAGUE_H

#include <assert.h>
#include <zlib.h>
#include "klib/kseq.h"
#include "cmdline.h"

KSEQ_INIT(gzFile, gzread)

void
read_fasta(char* filename, kseq_t* seq);

#endif
