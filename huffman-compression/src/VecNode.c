#include "VecNode.h"

VecNode *VecNode_init(size_t capacity) {
    VecNode *self = (VecNode *)malloc(sizeof(VecNode));
    if (self == NULL) {
        perror("VecNode_init");
        exit(EXIT_FAILURE);
    }

    self->items = (Node **)malloc(sizeof(Node *) * capacity);
    if (self->items == NULL) {
        perror("VecNode_init");
        exit(EXIT_FAILURE);
    }

    self->len = 0;
    return self;
}

void VecNode_destroy(VecNode **self) {
    free((*self)->items);
    free(*self);
    *self = NULL;
}

void VecNode_insert_at_index(VecNode *self, size_t index, Node *item) {
    for (size_t i = self->len; i > index; i--) {
        self->items[i] = VecNode_at(self, i - 1);
    }

    self->items[index] = item;
    self->len++;
}

void VecNode_push(VecNode *self, Node *item) {
    self->items[self->len] = item;
    self->len++;
}

Node *VecNode_at(VecNode *self, size_t index) {
    return self->items[index];
}

void VecNode_delete_at_index(VecNode *self, size_t index) {
    for (size_t i = index; i + 1 < self->len; i++) {
        self->items[i] = VecNode_at(self, i + 1);
    }

    self->len--;
}

Node *VecNode_pop(VecNode *self, size_t index) {
    Node *node = VecNode_at(self, index);
    VecNode_delete_at_index(self, index);
    return node;
}
