#ifndef VECNODE_H
#define VECNODE_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Node.h"

typedef struct VecNode {
    Node **items;
    size_t len;
} VecNode;

VecNode *VecNode_init(size_t capacity);

void VecNode_destroy(VecNode **self);

void VecNode_insert_at_index(VecNode *self, size_t index, Node *item);

void VecNode_push(VecNode *self, Node *item);

Node *VecNode_at(VecNode *self, size_t index);

void VecNode_delete_at_index(VecNode *self, size_t index);

Node *VecNode_pop(VecNode *self, size_t index);

#endif
