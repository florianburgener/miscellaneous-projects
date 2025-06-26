/**
 * @file BPTree.h
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#ifndef BPTREE_H
#define BPTREE_H

#include <stdbool.h>
#include <stdint.h>

#include "Array.h"

/**
 * @brief Data structure that represents a B+ Tree.
 *
 */
typedef struct BPTreeNode {
    int order;
    bool is_leaf;
    IntegerArray *keys;
    IntegerArray *data;
    BPTreeNodeArray *children;
    struct BPTreeNode *next;
} BPTreeNode;

/**
 * @brief Initializes a B+ Tree.
 *
 * @param order The order of the B+ Tree.
 * @return BPTreeNode* An empty B+ Tree.
 */
BPTreeNode *BPTree_init(int order);

/**
 * @brief Destroys the B+ Tree and free its memory.
 *
 * @param root The root of the B+ Tree.
 */
void BPTree_destroy(BPTreeNode **root);

/**
 * @brief Displays the B+ Tree.
 *
 * @param root The root of the B+ Tree.
 * @param depth Always pass 0 to this variable.
 */
void BPTree_print(BPTreeNode *root, int depth);

/**
 * @brief Searches for a key in the B+ Tree.
 *
 * @param root The root of the B+ Tree.
 * @param key The key to search.
 * @param data The data found will be assigned to this variable.
 * @return true  The key exists in the B+ tree.
 * @return false The key does not exist in the B+ Tree.
 */
bool BPTree_search(BPTreeNode *root, uint64_t key, uint64_t *data);

/**
 * @brief Inserts a key in the B+ Tree.
 *
 * @param root The root of the B+ Tree.
 * @param key The key to insert.
 * @param data The data to be inserted with the key.
 * @return true The key has been inserted.
 * @return false The key has not been inserted.
 */
bool BPTree_insert(BPTreeNode *root, uint64_t key, uint64_t data);

/**
 * @brief Deletes a key from the B+ Tree.
 *
 * @param root The root of the B+ Tree.
 * @param key The key to be deleted.
 * @return true The key has been deleted.
 * @return false The key has not been deleted.
 */
bool BPTree_delete(BPTreeNode *root, uint64_t key);

#endif
