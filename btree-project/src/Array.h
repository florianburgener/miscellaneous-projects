/**
 * @file Array.h
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#ifndef ARRAY_H
#define ARRAY_H

#include <stdbool.h>
#include <stdint.h>

// IntegerArray

/**
 * @brief Data structure that represents an array of integers of type uint64_t.
 *
 */
typedef struct IntegerArray {
    uint64_t *items;
    int size;
} IntegerArray;

/**
 * @brief Initializes the "IntegerArray" data structure.
 *
 * @param capacity Maximum number of items the array can contain.
 * @return IntegerArray* An empty "IntegerArray".
 */
IntegerArray *IntegerArray_init(int capacity);

/**
 * @brief Destroys the array and free its memory.
 *
 * @param array The array to be destroyed.
 */
void IntegerArray_destroy(IntegerArray **array);

/**
 * @brief Inserts an item in the array at the desired index.
 *
 * @param array The array in which the item will be inserted.
 * @param index The index where the item must be inserted.
 * @param item The item to be inserted.
 */
void IntegerArray_insert_at_index(IntegerArray *array, int index, uint64_t item);

/**
 * @brief Finds out at which index the item should be inserted in the array.
 *
 * @param array The array in which to search, it must be sorted.
 * @param item The item that should be inserted.
 * @return int The index where the item would be inserted.
 */
int IntegerArray_lower_bound(IntegerArray *array, uint64_t item);

/**
 * @brief Inserts an item in the sorted array.
 *
 * @param array The sorted array in which the item will be inserted.
 * @param item The item to be inserted.
 * @return int The index where the item has been inserted.
 */
int IntegerArray_insert_sorted(IntegerArray *array, uint64_t item);

/**
 * @brief Appends an item to the end of the array.
 *
 * @param array The array in which the item will be added.
 * @param item The item to be added.
 */
void IntegerArray_append(IntegerArray *array, uint64_t item);

/**
 * @brief Searches for an item in the sorted array.
 *
 * @param array The array in which the item is searched.
 * @param item The sought-after item.
 * @param index If the item is found its index will be assigned to this variable.
 * @return true The item has been found.
 * @return false The item has not been found.
 */
bool IntegerArray_binary_search(IntegerArray *array, uint64_t item, int *index);

/**
 * @brief Deletes an item from the array at the desired index.
 *
 * @param array The array in which the item is deleted.
 * @param index The index where the item must be deleted.
 */
void IntegerArray_delete_at_index(IntegerArray *array, int index);

/**
 * @brief Displays the array on the console.
 *
 * @param array The array to display.
 */
void IntegerArray_print(IntegerArray *array);

/**
 * @brief Deletes all items from the array.
 *
 * @param array The array where all the items must be deleted.
 */
void IntegerArray_clear(IntegerArray *array);

/**
 * @brief Copies the source array to the destination array.
 *
 * @param src The source array.
 * @param dest The destination array. The destination array must have at least the capacity of the source array.
 */
void IntegerArray_copy(IntegerArray *src, IntegerArray *dest);

// BPTreeNodeArray

typedef struct BPTreeNode BPTreeNode;

/**
 * @brief Data structure that represents an array of pointers of type BPTreeNode.
 *
 */
typedef struct BPTreeNodeArray {
    BPTreeNode **items;
    int size;
} BPTreeNodeArray;

/**
 * @brief Initializes the "BPTreeNodeArray" data structure.
 *
 * @param capacity Maximum number of items the array can contain.
 * @return BPTreeNodeArray* An empty "BPTreeNodeArray".
 */
BPTreeNodeArray *BPTreeNodeArray_init(int capacity);

/**
 * @brief Destroys the array and free its memory.
 *
 * @param array The array to be destroyed.
 */
void BPTreeNodeArray_destroy(BPTreeNodeArray **array);

/**
 * @brief Inserts an item in the array at the desired index.
 *
 * @param array The array in which the item will be inserted.
 * @param index The index where the item must be inserted.
 * @param item The item to be inserted.
 */
void BPTreeNodeArray_insert_at_index(BPTreeNodeArray *array, int index, BPTreeNode *item);

/**
 * @brief Appends an item to the end of the array.
 *
 * @param array The array in which the item will be added.
 * @param item The item to be added.
 */
void BPTreeNodeArray_append(BPTreeNodeArray *array, BPTreeNode *item);

/**
 * @brief Searches for an item in the array.
 *
 * @param array The array in which the item is searched.
 * @param item The sought-after item.
 * @return int If the item is found returns its index otherwise -1.
 */
int BPTreeNodeArray_search(BPTreeNodeArray *array, BPTreeNode *item);

/**
 * @brief Deletes an item from the array at the desired index.
 *
 * @param array The array in which the item is deleted.
 * @param index The index where the item must be deleted.
 */
void BPTreeNodeArray_delete_at_index(BPTreeNodeArray *array, int index);

/**
 * @brief Deletes all items from the array.
 *
 * @param array The array where all the items must be deleted.
 */
void BPTreeNodeArray_clear(BPTreeNodeArray *array);

/**
 * @brief Copies the source array to the destination array.
 *
 * @param src The source array.
 * @param dest The destination array. The destination array must have at least the capacity of the source array.
 */
void BPTreeNodeArray_copy(BPTreeNodeArray *src, BPTreeNodeArray *dest);

// ByteArray

/**
 * @brief Data structure that represents an array of bytes.
 *
 */
typedef struct ByteArray {
    uint8_t *items;
    int size;
} ByteArray;

/**
 * @brief Initializes the "ByteArray" data structure.
 *
 * @param capacity Maximum number of items the array can contain.
 * @return ByteArray* An empty "ByteArray".
 */
ByteArray *ByteArray_init(int capacity);

/**
 * @brief Destroys the array and free its memory.
 *
 * @param array The array to be destroyed.
 */
void ByteArray_destroy(ByteArray **array);

/**
 * @brief Appends an item to the end of the array.
 *
 * @param array The array in which the item will be added.
 * @param item The item to be added.
 */
void ByteArray_append(ByteArray *array, uint8_t item);

#endif
