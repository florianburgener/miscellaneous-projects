#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdint.h>
#include <string.h>

#include "Node.h"
#include "String.h"
#include "VecNode.h"
#include "io.h"

void compress(char *input_path, char *output_path);

void decompress(char *input_path, char *output_path);

#endif
