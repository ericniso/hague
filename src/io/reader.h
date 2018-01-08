#ifndef HAGUE_READER_H
#define HAGUE_READER_H

#include <assert.h>
#include <zlib.h>
#include "klib/kseq.h"

KSEQ_INIT(gzFile, gzread)

kseq_t*
read_fasta(char*, gzFile*);

#endif
