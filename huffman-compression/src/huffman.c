#include "huffman.h"

static uint32_t *count_chars(String *contents);
static void sorted_insert_by_frequency(VecNode *nodes, Node *new_node);
static Node *create_huffman_tree(uint32_t occurrences[256]);
static void get_leaf_nodes(Node *root, struct VecNode *nodes);
static VecU8 *compress_contents(Node *root, String *contents);
static Node *create_huffman_tree_from_leafs(VecNode *nodes);

void compress(char *input_path, char *output_path) {
    String *contents = read_text_file(input_path);
    uint32_t *char_counts = count_chars(contents);

    Node *root = create_huffman_tree(char_counts);

    VecNode *nodes = VecNode_init(256);
    get_leaf_nodes(root, nodes);
    VecU8 *bitstream = compress_contents(root, contents);

    write_compressed_file(output_path, nodes, bitstream);

    String_destroy(&contents);
    free(char_counts);
    Node_destroy(&root);
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
        if (VecU8_at(bitstream, i) == 0) {
            current = current->left;
        } else {
            current = current->right;
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
    Node_destroy(&root);
    String_destroy(&contents);
}

static uint32_t *count_chars(String *contents) {
    uint32_t *char_counts = (uint32_t *)malloc(sizeof(uint32_t) * 256);

    for (size_t i = 0; i < 256; i++) {
        char_counts[i] = 0;
    }

    for (size_t i = 0; i < contents->len; i++) {
        char c = String_at(contents, i);
        char_counts[(size_t)c] += 1;
    }

    return char_counts;
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

static Node *create_huffman_tree(uint32_t occurrences[256]) {
    VecNode *nodes = VecNode_init(256);

    for (size_t i = 0; i < 256; i++) {
        if (occurrences[i] > 0) {
            Node *node = Node_init();
            node->value = (char)i;
            node->frequency = occurrences[i];
            sorted_insert_by_frequency(nodes, node);
        }
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

    for (size_t i = 0; i < contents->len; i++) {
        VecU8 *binary = Node_find_binary(root, String_at(contents, i));

        for (size_t j = 0; j < binary->len; j++) {
            VecU8_push(bitstream, VecU8_at(binary, j));
        }

        VecU8_destroy(&binary);
    }

    return bitstream;
}

static Node *create_huffman_tree_from_leafs(VecNode *nodes) {
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

    Node *root = VecNode_pop(nodes, 0);
    return root;
}
