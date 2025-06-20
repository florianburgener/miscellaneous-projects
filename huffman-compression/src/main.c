#include <stdio.h>

#include "huffman.h"

int main(int argc, char *argv[]) {
#ifdef EXEC_COMPRESS
    if (argc != 3) {
        fprintf(stderr, "Usage: compress [INPUT_FILE] [OUTPUT_FILE]\n");
        exit(EXIT_FAILURE);
    }

    compress(argv[1], argv[2]);
#endif
#ifdef EXEC_DECOMPRESS
    if (argc != 3) {
        fprintf(stderr, "Usage: decompress [INPUT_FILE] [OUTPUT_FILE]\n");
        exit(EXIT_FAILURE);
    }

    decompress(argv[1], argv[2]);
#endif
    return EXIT_SUCCESS;
}
