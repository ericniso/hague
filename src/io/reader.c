#include "reader.h"

void
read_fasta(char* filename, kseq_t* seq)
{
    gzFile fp;
    fp = gzopen(filename, "r");
    assert(fp != NULL && "Could not open fasta file");
    seq = kseq_init(fp);
    gzclose(fp);
}
