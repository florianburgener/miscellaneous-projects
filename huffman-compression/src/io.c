#include "io.h"

static uint8_t read_byte(FILE *file);
static void write_byte(FILE *file, uint8_t value);
static void write_bitstream(FILE *file, VecU8 *bitstream);
static VecU8 *read_bitstream(FILE *file, size_t bitstream_len);

String *read_text_file(char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("read_text_file");
        exit(EXIT_FAILURE);
    }

    String *contents = String_init(1024);

    int c;
    while ((c = fgetc(file)) != EOF) {
        char str[2] = {0};
        str[0] = c;
        String_concat(contents, str);
    }

    if (fclose(file) != 0) {
        perror("read_text_file");
        exit(EXIT_FAILURE);
    }

    return contents;
}

void write_compressed_file(char *path, VecNode *nodes, VecU8 *bitstream) {
    FILE *file = fopen(path, "wb");
    if (file == NULL) {
        perror("write_compressed_file");
        exit(EXIT_FAILURE);
    }

    // The first byte represents the number of leaves in the Huffman tree.
    write_byte(file, nodes->len);

    // Write the nodes that will be used to reconstruct the Huffman tree.
    // For each leaf, we store its value (the character to be used during decompression) and its depth.
    for (size_t i = 0; i < nodes->len; i++) {
        write_byte(file, (uint8_t)VecNode_at(nodes, i)->value);
        write_byte(file, (uint8_t)VecNode_at(nodes, i)->depth);
    }

    // The next 4 bytes represent the size of the compressed data in bits.
    write_byte(file, bitstream->len & 0xFF);
    write_byte(file, (bitstream->len >> 8) & 0xFF);
    write_byte(file, (bitstream->len >> 16) & 0xFF);
    write_byte(file, (bitstream->len >> 24) & 0xFF);

    // Write the compressed data.
    write_bitstream(file, bitstream);

    if (fclose(file) != 0) {
        perror("write_compressed_file");
        exit(EXIT_FAILURE);
    }
}

void read_compressed_file(char *path, VecNode **nodes, VecU8 **bitstream) {
    FILE *file = fopen(path, "rb");
    if (file == NULL) {
        perror("read_compressed_file");
        exit(EXIT_FAILURE);
    }

    // The first byte represents the number of leaves in the Huffman tree.
    size_t nodes_len = (size_t)read_byte(file);
    *nodes = VecNode_init(nodes_len);

    // Read the nodes to reconstruct the Huffman tree.
    for (size_t i = 0; i < nodes_len; i++) {
        Node *node = Node_init();
        node->value = (char)read_byte(file);
        node->depth = read_byte(file);
        VecNode_push(*nodes, node);
    }

    // The next 4 bytes represent the size of the compressed data in bits.
    size_t bitstream_len = 0;
    for (size_t i = 0; i < 4; i++) {
        bitstream_len += ((size_t)read_byte(file)) << (i * 8);
    }

    // Read the compressed data.
    *bitstream = read_bitstream(file, bitstream_len);

    if (fclose(file) != 0) {
        perror("read_compressed_file");
        exit(EXIT_FAILURE);
    }
}

void write_text_file(char *path, String *contents) {
    FILE *file = fopen(path, "w");
    if (file == NULL) {
        perror("write_text_file");
        exit(EXIT_FAILURE);
    }

    for (size_t i = 0; i < contents->len; i++) {
        write_byte(file, (uint8_t)String_at(contents, i));
    }

    if (fclose(file) != 0) {
        perror("write_text_file");
        exit(EXIT_FAILURE);
    }
}

// TODO: better handle the return code of fread
static uint8_t read_byte(FILE *file) {
    uint8_t value;
    if (fread(&value, 1, 1, file) != 1) {
        perror("read_byte");
        exit(EXIT_FAILURE);
    }

    return value;
}

// TODO: better handle the return code of fwrite
static void write_byte(FILE *file, uint8_t value) {
    if (fwrite(&value, 1, 1, file) != 1) {
        perror("write_byte");
        exit(EXIT_FAILURE);
    }
}

static void write_bitstream(FILE *file, VecU8 *bitstream) {
    for (size_t i = 0; i < bitstream->len; i += 8) {
        uint8_t byte = 0;

        for (size_t j = 0; j < 8 && (i + j) < bitstream->len; j++) {
            byte |= VecU8_at(bitstream, i + j) << (7 - j);
        }

        write_byte(file, byte);
    }
}

static VecU8 *read_bitstream(FILE *file, size_t bitstream_len) {
    VecU8 *bitstream = VecU8_init(bitstream_len);

    for (size_t i = 0; i < bitstream_len; i += 8) {
        uint8_t byte = read_byte(file);

        for (size_t j = 0; j < 8 && (i + j) < bitstream_len; j++) {
            VecU8_push(bitstream, (byte >> (7 - j)) & 1);
        }
    }

    return bitstream;
}
