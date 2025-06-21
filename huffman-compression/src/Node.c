#include "Node.h"

static void _Node_print(Node *self, size_t padding);
static void _Node_compute_depths(Node *self, uint32_t depth);
static bool _Node_find_binary(Node *self, char c, VecU8 *binary);

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

bool Node_find_binary(Node *self, char c, VecU8 **binary) {
    // The maximum length of the binary is ASCII_EXTENDED_SIZE-1 (255) in the worst case, because the maximum number of leaf nodes is 256 (ASCII_EXTENDED_SIZE).
    // This is due to the way the Huffman tree is defined.
    *binary = VecU8_init(ASCII_EXTENDED_SIZE);

    if (_Node_find_binary(self, c, *binary)) {
        return true;
    }

    VecU8_destroy(binary);
    return false;
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

static bool _Node_find_binary(Node *self, char c, VecU8 *binary) {
    if (Node_is_leaf(self)) {
        if (self->value == c) {
            return true;
        }

        return false;
    }

    VecU8_push(binary, 0);

    if (_Node_find_binary(self->left, c, binary)) {
        return true;
    }

    binary->items[binary->len - 1] = 1;

    if (_Node_find_binary(self->right, c, binary)) {
        return true;
    }

    VecU8_pop(binary, binary->len - 1);
    return false;
}
