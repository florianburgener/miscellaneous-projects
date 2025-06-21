#include "huffman.h"

static uint32_t *count_characters(String *contents);
static void sorted_insert_by_frequency(VecNode *nodes, Node *new_node);
static Node *create_huffman_tree(String *contents);
static void get_leaf_nodes(Node *root, struct VecNode *nodes);
static VecU8 *compress_contents(Node *root, String *contents);
static Node *create_huffman_tree_from_leafs(VecNode *nodes);

void compress(char *input_path, char *output_path) {
    String *contents = read_text_file(input_path);
    Node *root = create_huffman_tree(contents);

    VecNode *nodes = VecNode_init(ASCII_EXTENDED_SIZE);
    get_leaf_nodes(root, nodes);
    VecU8 *bitstream = compress_contents(root, contents);

    write_compressed_file(output_path, nodes, bitstream);

    String_destroy(&contents);
    if (root != NULL) {
        Node_destroy(&root);
    }
    VecNode_destroy(&nodes);
    VecU8_destroy(&bitstream);
}

void decompress(char *input_path, char *output_path) {
    VecNode *nodes;
    VecU8 *bitstream;
    read_compressed_file(input_path, &nodes, &bitstream);

    Node *root = create_huffman_tree_from_leafs(nodes);
    Node *current = root;
    String *contents = String_init(1024);

    for (size_t i = 0; i < bitstream->len; i++) {
        if (!Node_is_leaf(root)) {
            // We only move through the tree if the root node is not a leaf.
            // If it is a leaf, this means that only one type of character has been compressed.
            if (VecU8_at(bitstream, i) == 0) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        if (Node_is_leaf(current)) {
            char str[2] = {0};
            str[0] = current->value;
            String_concat(contents, str);
            current = root;
        }
    }

    write_text_file(output_path, contents);

    VecNode_destroy(&nodes);
    VecU8_destroy(&bitstream);
    if (root != NULL) {
        Node_destroy(&root);
    }
    String_destroy(&contents);
}

static uint32_t *count_characters(String *contents) {
    uint32_t *char_count = (uint32_t *)malloc(sizeof(uint32_t) * ASCII_EXTENDED_SIZE);

    for (size_t i = 0; i < ASCII_EXTENDED_SIZE; i++) {
        char_count[i] = 0;
    }

    for (size_t i = 0; i < contents->len; i++) {
        char c = String_at(contents, i);
        char_count[(size_t)c] += 1;
    }

    return char_count;
}

static void sorted_insert_by_frequency(VecNode *nodes, Node *new_node) {
    size_t index = nodes->len;

    for (size_t i = 0; i < nodes->len; i++) {
        if (new_node->frequency <= VecNode_at(nodes, i)->frequency) {
            index = i;
            break;
        }
    }

    VecNode_insert_at_index(nodes, index, new_node);
}

static Node *create_huffman_tree(String *contents) {
    uint32_t *char_count = count_characters(contents);
    VecNode *nodes = VecNode_init(ASCII_EXTENDED_SIZE);

    for (size_t i = 0; i < ASCII_EXTENDED_SIZE; i++) {
        if (char_count[i] > 0) {
            Node *node = Node_init();
            node->value = (char)i;
            node->frequency = char_count[i];
            sorted_insert_by_frequency(nodes, node);
        }
    }

    free(char_count);

    if (nodes->len == 0) {
        VecNode_destroy(&nodes);
        return NULL;
    }

    while (nodes->len != 1) {
        Node *parent = Node_init();
        parent->left = VecNode_pop(nodes, 0);
        parent->right = VecNode_pop(nodes, 0);
        parent->frequency = parent->left->frequency + parent->right->frequency;

        sorted_insert_by_frequency(nodes, parent);
    }

    Node *root = VecNode_pop(nodes, 0);
    VecNode_destroy(&nodes);
    Node_compute_depths(root);
    return root;
}

static void get_leaf_nodes(Node *root, struct VecNode *nodes) {
    if (root == NULL) {
        return;
    }

    get_leaf_nodes(root->left, nodes);
    get_leaf_nodes(root->right, nodes);

    if (Node_is_leaf(root)) {
        VecNode_push(nodes, root);
    }
}

static VecU8 *compress_contents(Node *root, String *contents) {
    // Compressed content will never be larger than its original size, hence the capacity being the original size.
    VecU8 *bitstream = VecU8_init(contents->len * 8);

    if (root == NULL) {
        return bitstream;
    }

    VecU8 *binary_by_char[ASCII_EXTENDED_SIZE];

    for (size_t i = 0; i < ASCII_EXTENDED_SIZE; i++) {
        if (!Node_find_binary(root, (char)i, &binary_by_char[i])) {
            continue;
        }

        // Special case when the file contains only one type of character.
        if (binary_by_char[i]->len == 0 && Node_is_leaf(root)) {
            VecU8_push(binary_by_char[i], 0);
        }
    }

    for (size_t i = 0; i < contents->len; i++) {
        VecU8 *binary = binary_by_char[(size_t)String_at(contents, i)];

        for (size_t j = 0; j < binary->len; j++) {
            VecU8_push(bitstream, VecU8_at(binary, j));
        }
    }

    for (size_t i = 0; i < ASCII_EXTENDED_SIZE; i++) {
        if (binary_by_char[i] != NULL) {
            VecU8_destroy(&binary_by_char[i]);
        }
    }

    return bitstream;
}

static Node *create_huffman_tree_from_leafs(VecNode *nodes) {
    if (nodes->len == 0) {
        return NULL;
    }

    uint8_t current_depth = 0;

    for (size_t i = 0; i < nodes->len; i++) {
        if (VecNode_at(nodes, i)->depth > current_depth) {
            current_depth = VecNode_at(nodes, i)->depth;
        }
    }

    while (nodes->len != 1) {
        size_t index = nodes->len;
        bool found = false;

        while (!found) {
            for (size_t i = 0; i < nodes->len; i++) {
                if (VecNode_at(nodes, i)->depth == current_depth) {
                    index = i;
                    found = true;
                    break;
                }
            }

            if (found) {
                break;
            }

            current_depth -= 1;
        }

        Node *parent = Node_init();
        parent->left = VecNode_pop(nodes, index);
        parent->right = VecNode_pop(nodes, index);
        parent->depth = current_depth - 1;

        VecNode_insert_at_index(nodes, index, parent);
    }

    return VecNode_pop(nodes, 0);
}
