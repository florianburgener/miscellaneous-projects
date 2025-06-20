#include "Node.h"

static void _Node_print(Node *self, size_t padding);
static void _Node_compute_depths(Node *self, uint32_t depth);
static VecU8 *_Node_find_binary(Node *self, char c, VecU8 *binary);

Node *Node_init() {
    Node *self = (Node *)malloc(sizeof(Node));
    if (self == NULL) {
        perror("Node_init");
        exit(EXIT_FAILURE);
    }

    self->left = NULL;
    self->right = NULL;
    self->value = '\0';
    self->depth = 0;
    self->frequency = 0;
    return self;
}

void Node_destroy(Node **self) {
    if ((*self)->left != NULL) {
        Node_destroy(&(*self)->left);
    }

    if ((*self)->right != NULL) {
        Node_destroy(&(*self)->right);
    }

    free(*self);
    *self = NULL;
}

void Node_print(Node *self) {
    _Node_print(self, 0);
}

void Node_compute_depths(Node *self) {
    _Node_compute_depths(self, 0);
}

VecU8 *Node_find_binary(Node *self, char c) {
    VecU8 *binary = VecU8_init(256);
    return _Node_find_binary(self, c, binary);
}

bool Node_is_leaf(Node *self) {
    return self->left == NULL && self->right == NULL;
}

static void _Node_print(Node *self, size_t padding) {
    if (self == NULL) {
        return;
    }

    for (size_t i = 0; i < padding; i++) {
        printf(" ");
    }

    printf("%p", self);

    if (self->value != '\0') {
        printf(" (%c)", self->value);
    }

    printf(" D=%u\n", self->depth);
    _Node_print(self->left, padding + 2);
    _Node_print(self->right, padding + 2);
}

static void _Node_compute_depths(Node *self, uint32_t depth) {
    if (self == NULL) {
        return;
    }

    self->depth = depth;

    _Node_compute_depths(self->left, depth + 1);
    _Node_compute_depths(self->right, depth + 1);
}

static VecU8 *_Node_find_binary(Node *self, char c, VecU8 *binary) {
    if (self == NULL) {
        VecU8_destroy(&binary);
        return NULL;
    }

    if (self->value == c) {
        return binary;
    }

    VecU8 *binary_copy;
    VecU8 *result;

    binary_copy = VecU8_init(256);
    VecU8_copy(binary, binary_copy);
    VecU8_push(binary_copy, 0);
    result = _Node_find_binary(self->left, c, binary_copy);
    if (result != NULL) {
        VecU8_destroy(&binary);
        return result;
    }

    binary_copy = VecU8_init(256);
    VecU8_copy(binary, binary_copy);
    VecU8_push(binary_copy, 1);
    result = _Node_find_binary(self->right, c, binary_copy);
    if (result != NULL) {
        VecU8_destroy(&binary);
        return result;
    }

    VecU8_destroy(&binary);
    return NULL;
}
