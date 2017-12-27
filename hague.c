#include "hague.h"

int main(int argc, char** argv)
{
    ggo_args ai;
    assert(cmdline_parser(argc, argv, &ai) == 0);

    return 0;
}

void
read_fasta(char* filename, kseq_t* seq)
{
    gzFile fp;
    fp = gzopen(filename, "r");
    assert(fp != NULL && "Could not open fasta file");
    seq = kseq_init(fp);
    gzclose(fp);
}
