#ifndef HAGUE_READER_H
#define HAGUE_READER_H

#include <assert.h>
#include <zlib.h>
#include "../../libs/klib/kseq.h"

KSEQ_INIT(gzFile, gzread)

void
read_fasta(char*, kseq_t*);

#endif
