/**
 * @file BPTree.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include "BPTree.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Array.h"

// BPTreeNode

/**
 * @brief Initializes the "BPTreeNode" data structure.
 *
 * @param order The order of the node.
 * @param is_leaf true = the node is a leaf, false = the node is not a leaf.
 * @return BPTreeNode* The initialized node.
 */
static BPTreeNode *BPTreeNode_init(int order, bool is_leaf) {
    BPTreeNode *root = (BPTreeNode *)malloc(sizeof(BPTreeNode));
    root->order = order;
    root->is_leaf = is_leaf;
    root->keys = IntegerArray_init(2 * root->order);
    root->data = IntegerArray_init(2 * root->order);
    // The length of the children's array is always 1 greater than the keys.
    root->children = BPTreeNodeArray_init(2 * root->order + 1);
    root->next = NULL;
    return root;
}

/**
 * @brief Destroys the node and free its memory.
 *
 * @param node The node to be destroyed.
 */
static void BPTreeNode_destroy(BPTreeNode **node) {
    IntegerArray_destroy(&(*node)->keys);
    IntegerArray_destroy(&(*node)->data);
    BPTreeNodeArray_destroy(&(*node)->children);
    free(*node);
    *node = NULL;
}

// BPTree

BPTreeNode *BPTree_init(int order) {
    return BPTreeNode_init(order, true);
}

void BPTree_destroy(BPTreeNode **root) {
    for (int i = 0; i < (*root)->children->size; i++) {
        BPTree_destroy(&(*root)->children->items[i]);
    }

    BPTreeNode_destroy(root);
}

void BPTree_print(BPTreeNode *root, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("    ");
    }
    printf("%p\n", (void *)root);

    for (int i = 0; i < depth + 1; i++) {
        printf("    ");
    }
    IntegerArray_print(root->keys);

    for (int i = 0; i < depth + 1; i++) {
        printf("    ");
    }
    printf("Data = ");
    IntegerArray_print(root->data);

    for (int i = 0; i < depth + 1; i++) {
        printf("    ");
    }
    printf("Next = %p\n", (void *)root->next);

    for (int i = 0; i < root->children->size; i++) {
        BPTree_print(root->children->items[i], depth + 1);
    }
}

bool BPTree_search(BPTreeNode *root, uint64_t key, uint64_t *data) {
    if (root->is_leaf) {
        int index;
        bool found = IntegerArray_binary_search(root->keys, key, &index);

        if (found) {
            *data = root->data->items[index];
        }

        return found;
    }

    int index_in_children = IntegerArray_lower_bound(root->keys, key);

    if (index_in_children < root->keys->size && root->keys->items[index_in_children] == key) {
        index_in_children += 1;
    }

    return BPTree_search(root->children->items[index_in_children], key, data);
}

// "traverse" function for insertion and deletion.

/**
 * @brief Finds out which child node to traverse based on the key.
 *
 * @param node The origin node.
 * @param key The key that must be compared.
 * @return BPTreeNode* The next node.
 */
static BPTreeNode *traverse(BPTreeNode *node, uint64_t key) {
    int virtual_insertion_index = IntegerArray_lower_bound(node->keys, key);

    if (virtual_insertion_index < node->keys->size && node->keys->items[virtual_insertion_index] == key) {
        // If the key is equal to the place where it should be inserted, it is necessary to take the child to the right of it.
        virtual_insertion_index += 1;
    }

    return node->children->items[virtual_insertion_index];
}

// BPTree : Insertion

static void insert_non_full(BPTreeNode *node, uint64_t key, uint64_t data, BPTreeNode *previous_split_right_node) {
    int insertion_index = IntegerArray_insert_sorted(node->keys, key);

    if (node->is_leaf) {
        IntegerArray_insert_at_index(node->data, insertion_index, data);
    }

    if (previous_split_right_node != NULL) {
        BPTreeNodeArray_insert_at_index(node->children, insertion_index + 1, previous_split_right_node);
    }
}

/**
 * @brief Redistributes the keys. The right node is empty and the left node contains all the informations.
 *
 * @param left_node The left node.
 * @param right_node The right node.
 * @param left_index The informations is cut in the left node from the left_index. The information in the left_index is not included.
 * @param right_index The information is cut in the left node from the right index.
 */
static void redistribute_keys(BPTreeNode *left_node, BPTreeNode *right_node, int left_index, int right_index) {
    for (int i = right_index; i < left_node->keys->size; i++) {
        IntegerArray_append(right_node->keys, left_node->keys->items[i]);
    }

    left_node->keys->size = left_index;

    // The data is also redistributed if there is any.
    for (int i = right_index; i < left_node->data->size; i++) {
        IntegerArray_append(right_node->data, left_node->data->items[i]);
    }

    if (left_node->data->size > 0) {
        left_node->data->size = left_index;
    }
}

/**
 * @brief Splits a leaf.
 *
 * @param node The node to split. It must be a leaf.
 * @param key The key to insert after splitting.
 * @param data The data to insert after splitting.
 * @param split_right_node In this variable is assigned the right node split.
 * @return uint64_t The median value resulting from the split.
 */
static uint64_t split_leaf(BPTreeNode *node, uint64_t key, uint64_t data, BPTreeNode **split_right_node) {
    int virtual_insertion_index = IntegerArray_lower_bound(node->keys, key);
    int median_index = node->keys->size / 2;
    uint64_t median_value;
    *split_right_node = BPTreeNode_init(node->order, true);

    if (virtual_insertion_index < median_index) {
        // The key is inserted to the left of the median value.
        median_value = node->keys->items[median_index - 1];
        redistribute_keys(node, *split_right_node, median_index - 1, median_index - 1);
        // Inserts the key and the data.
        int insertion_index = IntegerArray_insert_sorted(node->keys, key);
        IntegerArray_insert_at_index(node->data, insertion_index, data);
    } else if (virtual_insertion_index > median_index) {
        // The key is inserted to the right of the median value.
        median_value = node->keys->items[median_index];
        redistribute_keys(node, *split_right_node, median_index, median_index);
        // Inserts the key and the data.
        int insertion_index = IntegerArray_insert_sorted((*split_right_node)->keys, key);
        IntegerArray_insert_at_index((*split_right_node)->data, insertion_index, data);
    } else {
        // The key is inserted at exactly the place of the median value.
        median_value = key;
        redistribute_keys(node, *split_right_node, median_index, median_index);
        // Inserts the key and the data.
        int insertion_index = IntegerArray_insert_sorted((*split_right_node)->keys, key);
        IntegerArray_insert_at_index((*split_right_node)->data, insertion_index, data);
    }

    // Maintains the linked list of leaf nodes.
    if (node->next != NULL) {
        (*split_right_node)->next = node->next;
    }
    node->next = *split_right_node;

    return median_value;
}

/**
 * @brief Redistributes the children. The right node is empty and the left node contains all the informations.
 *
 * @param left_node The left node.
 * @param right_node The right node.
 * @param left_index The informations is cut in the left node from the left_index. The information in the left_index is not included.
 * @param right_index The information is cut in the left node from the right index.
 */
static void redistribute_children(BPTreeNode *left_node, BPTreeNode *right_node, int left_index, int right_index) {
    for (int i = right_index; i < left_node->children->size; i++) {
        BPTreeNodeArray_append(right_node->children, left_node->children->items[i]);
    }

    left_node->children->size = left_index;
}

/**
 * @brief Splits an internal node.
 *
 * @param node The node to split. It must be an internal node.
 * @param key The key to insert after splitting.
 * @param previous_split_right_node The node resulting from a previous split that must be inserted after the split.
 * @param split_right_node In this variable is assigned the right node split.
 * @return uint64_t The median value resulting from the split.
 */
static uint64_t split_internal(BPTreeNode *node, uint64_t key, BPTreeNode *previous_split_right_node, BPTreeNode **split_right_node) {
    int virtual_insertion_index = IntegerArray_lower_bound(node->keys, key);
    int median_index = node->keys->size / 2;
    uint64_t median_value;
    *split_right_node = BPTreeNode_init(node->order, false);

    if (virtual_insertion_index < median_index) {
        // The key is inserted to the left of the median value.
        median_value = node->keys->items[median_index - 1];
        redistribute_keys(node, *split_right_node, median_index - 1, median_index);
        redistribute_children(node, *split_right_node, median_index, median_index);
        int insertion_index = IntegerArray_insert_sorted(node->keys, key);
        // previous_split_right_node is inserted to the right of the key in the child array.
        BPTreeNodeArray_insert_at_index(node->children, insertion_index + 1, previous_split_right_node);
    } else if (virtual_insertion_index > median_index) {
        // The key is inserted to the right of the median value.
        median_value = node->keys->items[median_index];
        redistribute_keys(node, *split_right_node, median_index, median_index + 1);
        redistribute_children(node, *split_right_node, median_index + 1, median_index + 1);
        int insertion_index = IntegerArray_insert_sorted((*split_right_node)->keys, key);
        // previous_split_right_node is inserted to the right of the key in the child array.
        BPTreeNodeArray_insert_at_index((*split_right_node)->children, insertion_index + 1, previous_split_right_node);
    } else {
        // The key is inserted at exactly the place of the median value.
        median_value = key;
        redistribute_keys(node, *split_right_node, median_index, median_index);
        redistribute_children(node, *split_right_node, median_index + 1, median_index + 1);
        // previous_split_right_node is always inserted at index 0 the array of children of split_right_node.
        BPTreeNodeArray_insert_at_index((*split_right_node)->children, 0, previous_split_right_node);
    }

    return median_value;
}

/**
 * @brief Inserts information when the node is full.
 *
 * @param node The node to split.
 * @param key The key to insert after splitting.
 * @param data The data to insert after splitting.
 * @param previous_split_right_node The node resulting from a previous split that must be inserted after the split.
 * @param split_right_node In this variable is assigned the right node split.
 * @return uint64_t The median value resulting from the split.
 */
static uint64_t insert_full(BPTreeNode *node, uint64_t key, uint64_t data, BPTreeNode *previous_split_right_node, BPTreeNode **split_right_node) {
    if (node->is_leaf) {
        return split_leaf(node, key, data, split_right_node);
    }

    return split_internal(node, key, previous_split_right_node, split_right_node);
}

/**
 * @brief Grows the tree.
 *
 * @param root The real root node.
 * @param median_value The median value of the split.
 * @param split_right_node The right node of the split.
 */
static void grow(BPTreeNode *root, uint64_t median_value, BPTreeNode *split_right_node) {
    // When the tree grows is necessarily no longer a leaf.
    root->is_leaf = false;
    BPTreeNode *left_node = BPTreeNode_init(root->order, split_right_node->is_leaf);
    // Maintains the linked list of leaf nodes.
    left_node->next = root->next;
    root->next = NULL;

    // Copy the data from the root to the left_node.
    IntegerArray_copy(root->keys, left_node->keys);
    IntegerArray_copy(root->data, left_node->data);
    BPTreeNodeArray_copy(root->children, left_node->children);

    IntegerArray_clear(root->keys);
    IntegerArray_clear(root->data);
    BPTreeNodeArray_clear(root->children);

    // Reorganizes the root node.
    IntegerArray_append(root->keys, median_value);
    BPTreeNodeArray_append(root->children, left_node);
    BPTreeNodeArray_append(root->children, split_right_node);
}

/**
 * @brief Insertion sub-function that performs the insertion recursively.
 *
 * @param parent The parent of the root node.
 * @param root The root node.
 * @param key The key to insert. A pointer is used for recursions.
 * @param data The data to be inserted with the key.
 * @param split_right_node A pointer is used for recursions.
 * @return true The key has been inserted.
 * @return false The key has not been inserted.
 */
static bool _BPTree_insert(BPTreeNode *parent, BPTreeNode *root, uint64_t *key, uint64_t data, BPTreeNode **split_right_node) {
    BPTreeNode *previous_split_right_node = NULL;
    if (!root->is_leaf && !_BPTree_insert(root, traverse(root, *key), key, data, &previous_split_right_node)) {
        // The previous recursion indicates that it is not possible to insert the key.
        return false;
    }

    // The first time the leaf is visited, it is necessary to check if it is possible to insert the key.
    int index;
    bool is_found = IntegerArray_binary_search(root->keys, *key, &index);
    if (root->is_leaf && is_found) {
        // The key cannot be inserted because it already exists.
        return false;
    }

    if (!root->is_leaf && previous_split_right_node == NULL) {
        // Prevents from reaching the code below because the insertion is finished.
        *split_right_node = NULL;
        return true;
    }

    if (root->keys->size < 2 * root->order) {
        // There is enough room in the root node to insert the key.
        insert_non_full(root, *key, data, previous_split_right_node);
        // End the insertions.
        *split_right_node = NULL;
        return true;
    }

    // Inserts and splits the root node.
    *key = insert_full(root, *key, data, previous_split_right_node, split_right_node);

    if (parent == NULL) {
        // If the root node is the real root, the tree must grow.
        grow(root, *key, *split_right_node);
    }

    return true;
}

bool BPTree_insert(BPTreeNode *root, uint64_t key, uint64_t data) {
    BPTreeNode *previous_split_right_node;
    return _BPTree_insert(NULL, root, &key, data, &previous_split_right_node);
}

// BPTree : Deletion

/**
 * @brief Finds the smallest key in the subtree.
 *
 * @param root The root node of a subtree.
 * @return uint64_t The smallest key of the subtree.
 */
static uint64_t find_smallest_key(BPTreeNode *root) {
    if (root->is_leaf) {
        return root->keys->items[0];
    }

    return find_smallest_key(root->children->items[0]);
}

/**
 * @brief Shrinks the height of the tree.
 *
 * @param root The real root node of the tree.
 */
static void shrink(BPTreeNode *root) {
    BPTreeNode *child = root->children->items[0];
    root->is_leaf = child->is_leaf;

    // Copies the information of the only child in the root.
    IntegerArray_copy(child->keys, root->keys);
    IntegerArray_copy(child->data, root->data);

    BPTreeNodeArray_clear(root->children);
    BPTreeNodeArray_copy(child->children, root->children);

    BPTreeNode_destroy(&child);
}

/**
 * @brief Finds the best sibling of the node.
 *
 * @param parent The parent of the node.
 * @param node The node.
 * @return BPTreeNode* The best sibling.
 */
static BPTreeNode *find_sibling(BPTreeNode *parent, BPTreeNode *node) {
    int index_in_children = BPTreeNodeArray_search(parent->children, node);

    if (index_in_children == 0) {
        // No other choice but to take the index 1.
        return parent->children->items[1];
    }

    if (index_in_children == parent->children->size - 1) {
        // No other choice but to take the index parent->children->size - 2.
        return parent->children->items[parent->children->size - 2];
    }

    if (parent->children->items[index_in_children - 1]->keys->size > parent->order) {
        // The left sibling has enough keys.
        return parent->children->items[index_in_children - 1];
    }

    if (parent->children->items[index_in_children + 1]->keys->size > parent->order) {
        // The right sibling has enough keys.
        return parent->children->items[index_in_children + 1];
    }

    // None of the siblings have enough keys, so the left sibling is chosen.
    return parent->children->items[index_in_children - 1];
}

/**
 * @brief Checks if the sibling is to the left side of the node.
 *
 * @param parent The parent of the node and the sibling.
 * @param node The node.
 * @param sibling The sibling.
 * @return true The sibling is to the left of the node.
 * @return false The sibling is not on the left of the node.
 */
static bool is_sibling_left_side(BPTreeNode *parent, BPTreeNode *node, BPTreeNode *sibling) {
    return BPTreeNodeArray_search(parent->children, sibling) < BPTreeNodeArray_search(parent->children, node);
}

/**
 * @brief Merges the left node and the right node.
 *
 * @param parent The parent of the left_node and right_node.
 * @param left_node The left node.
 * @param right_node The right node.
 */
static void merge(BPTreeNode *parent, BPTreeNode *left_node, BPTreeNode *right_node) {
    // The right node is always merged into the left node.
    int index_in_children = BPTreeNodeArray_search(parent->children, left_node);

    if (!left_node->is_leaf) {
        // If it is an internal node the key that links the left and right nodes must also be merge.
        IntegerArray_append(left_node->keys, parent->keys->items[index_in_children]);
    }

    // Merge the keys.
    for (int i = 0; i < right_node->keys->size; i++) {
        IntegerArray_append(left_node->keys, right_node->keys->items[i]);
    }

    // Merge the data.
    for (int i = 0; i < right_node->data->size; i++) {
        IntegerArray_append(left_node->data, right_node->data->items[i]);
    }

    // Merge the children.
    for (int i = 0; i < right_node->children->size; i++) {
        BPTreeNodeArray_append(left_node->children, right_node->children->items[i]);
    }

    // Deletes the correct information from the parent.
    IntegerArray_delete_at_index(parent->keys, index_in_children);
    BPTreeNodeArray_delete_at_index(parent->children, index_in_children + 1);

    // Maintains the linked list of leaf nodes.
    left_node->next = right_node->next;

    BPTreeNode_destroy(&right_node);
}

/**
 * @brief Steals a key from the sibling. The nodes are leaf.
 *
 * @param parent The parent of the node and the sibling.
 * @param node The node.
 * @param sibling The sibling.
 */
static void steal_leaf(BPTreeNode *parent, BPTreeNode *node, BPTreeNode *sibling) {
    int index_in_children = BPTreeNodeArray_search(parent->children, node);

    if (is_sibling_left_side(parent, node, sibling)) {
        // If the sibling is on the left the last key is stolen from the sibling.
        uint64_t stealed_key = sibling->keys->items[sibling->keys->size - 1];
        IntegerArray_insert_at_index(node->keys, 0, stealed_key);
        IntegerArray_delete_at_index(sibling->keys, sibling->keys->size - 1);
        parent->keys->items[index_in_children - 1] = stealed_key;

        // The data must also be stolen.
        IntegerArray_insert_at_index(node->data, 0, sibling->data->items[sibling->data->size - 1]);
        IntegerArray_delete_at_index(sibling->data, sibling->data->size - 1);
    } else {
        // If the sibling is on the right the first key is stolen from the sibling.
        uint64_t stealed_key = sibling->keys->items[0];
        IntegerArray_append(node->keys, stealed_key);
        IntegerArray_delete_at_index(sibling->keys, 0);
        parent->keys->items[index_in_children] = sibling->keys->items[0];

        // The data must also be stolen.
        IntegerArray_append(node->data, sibling->data->items[0]);
        IntegerArray_delete_at_index(sibling->data, 0);
    }
}

/**
 * @brief Steals a key from the sibling. The nodes are internal.
 *
 * @param parent The parent of the node and the sibling.
 * @param node The node.
 * @param sibling The sibling.
 */
static void steal_internal(BPTreeNode *parent, BPTreeNode *node, BPTreeNode *sibling) {
    int index_in_children = BPTreeNodeArray_search(parent->children, node);

    if (is_sibling_left_side(parent, node, sibling)) {
        // If the sibling is on the left the last key is stolen from the sibling.
        IntegerArray_insert_at_index(node->keys, 0, parent->keys->items[index_in_children - 1]);
        parent->keys->items[index_in_children - 1] = sibling->keys->items[sibling->keys->size - 1];
        IntegerArray_delete_at_index(sibling->keys, sibling->keys->size - 1);
        BPTreeNodeArray_insert_at_index(node->children, 0, sibling->children->items[sibling->children->size - 1]);
        BPTreeNodeArray_delete_at_index(sibling->children, sibling->children->size - 1);
    } else {
        // If the sibling is on the right the first key is stolen from the sibling.
        IntegerArray_append(node->keys, parent->keys->items[index_in_children]);
        parent->keys->items[index_in_children] = sibling->keys->items[0];
        IntegerArray_delete_at_index(sibling->keys, 0);
        BPTreeNodeArray_append(node->children, sibling->children->items[0]);
        BPTreeNodeArray_delete_at_index(sibling->children, 0);
    }
}

/**
 * @brief Rebalances the tree.
 *
 * @param parent The parent of the node.
 * @param node The node.
 */
static void deletion_rebalance(BPTreeNode *parent, BPTreeNode *node) {
    BPTreeNode *sibling = find_sibling(parent, node);

    if (sibling->keys->size == sibling->order) {
        // The sibling does not have enough keys to steal one, a merge is required.
        if (is_sibling_left_side(parent, node, sibling)) {
            merge(parent, sibling, node);
        } else {
            merge(parent, node, sibling);
        }
    } else if (node->is_leaf) {
        // Steals a key from a leaf.
        steal_leaf(parent, node, sibling);
    } else {
        // Steals a key from an internal node.
        steal_internal(parent, node, sibling);
    }
}

/**
 * @brief Sub-function of deletion that performs the deletion recursively.
 *
 * @param parent The parent of the root node.
 * @param root The root node.
 * @param key The key to delete.
 * @return true The key has been deleted.
 * @return false The key has not been deleted.
 */
static bool _BPTree_delete(BPTreeNode *parent, BPTreeNode *root, uint64_t key) {
    if (!root->is_leaf && !_BPTree_delete(root, traverse(root, key), key)) {
        // The previous recursion indicates that it is not possible to delete the key.
        return false;
    }

    // The first time the leaf is visited, it is necessary to check if it is possible to delete the key.
    int index;
    bool is_found = IntegerArray_binary_search(root->keys, key, &index);
    if (root->is_leaf && !is_found) {
        // The key cannot be deleted because it doesn't exists.
        return false;
    }

    if (root->is_leaf) {
        // The key is deleted from the leaf as well as its data.
        IntegerArray_delete_at_index(root->keys, index);
        IntegerArray_delete_at_index(root->data, index);
    } else if (is_found) {
        // The key must be replaced by the smallest key of the right subtree.
        root->keys->items[index] = find_smallest_key(root->children->items[index + 1]);
    }

    if (parent == NULL && !root->is_leaf && root->keys->size == 0) {
        // The real root node of the tree is empty, the tree must be shrink.
        shrink(root);
    }

    if (parent != NULL && root->keys->size < root->order) {
        // Rebalances of the tree after deletion.
        deletion_rebalance(parent, root);
    }

    return true;
}

bool BPTree_delete(BPTreeNode *root, uint64_t key) {
    return _BPTree_delete(NULL, root, key);
}
