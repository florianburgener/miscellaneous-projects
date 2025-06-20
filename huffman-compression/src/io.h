#ifndef IO_H
#define IO_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "String.h"
#include "VecNode.h"
#include "VecU8.h"

String *read_text_file(char *path);

void write_compressed_file(char *path, VecNode *nodes, VecU8 *bitstream);

void read_compressed_file(char *path, VecNode **nodes, VecU8 **bitstream);

void write_text_file(char *path, String *contents);

#endif
