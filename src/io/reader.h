#ifndef HAGUE_READER_H
#define HAGUE_READER_H

#include <assert.h>
#include <zlib.h>
#include "utils/initializer.h"
#include "klib/kseq.h"

/**
 * @brief Read FASTA file(gz compressed or uncompressed)
 *
 * @return kseq_t structure with name, comment, sequence and quality fields.
 */
kseq_t*
read_fasta(char*, gzFile*);

#endif
