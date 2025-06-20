#ifndef NODE_H
#define NODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "VecU8.h"

typedef struct Node {
    struct Node* left;
    struct Node* right;
    char value;
    uint8_t depth;
    uint32_t frequency;
} Node;

Node* Node_init();

void Node_destroy(Node** self);

void Node_print(Node* self);

void Node_compute_depths(Node* self);

VecU8* Node_find_binary(Node* self, char c);

bool Node_is_leaf(Node* self);

#endif
