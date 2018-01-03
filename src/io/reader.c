#include "reader.h"

kseq_t*
read_fasta(char* filename, gzFile* fp)
{
    *fp = gzopen(filename, "r");
    assert(fp != NULL && "Could not open fasta file");
    kseq_t* seq = kseq_init(*fp);

    return seq;
}
