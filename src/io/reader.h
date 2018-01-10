#ifndef HAGUE_READER_H
#define HAGUE_READER_H

#include <assert.h>
#include <zlib.h>
#include "utils/initializer.h"
#include "klib/kseq.h"

kseq_t*
read_fasta(char*, gzFile*);

#endif
