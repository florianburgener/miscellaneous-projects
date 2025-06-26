/**
 * @file Array.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include "Array.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// IntegerArray

IntegerArray *IntegerArray_init(int capacity) {
    IntegerArray *array = (IntegerArray *)malloc(sizeof(IntegerArray));
    array->items = (uint64_t *)malloc(sizeof(uint64_t) * capacity);
    array->size = 0;
    return array;
}

void IntegerArray_destroy(IntegerArray **array) {
    free((*array)->items);
    free(*array);
    *array = NULL;
}

void IntegerArray_insert_at_index(IntegerArray *array, int index, uint64_t item) {
    for (int i = array->size - 1; i >= index; i--) {
        array->items[i + 1] = array->items[i];
    }

    array->items[index] = item;
    array->size++;
}

int IntegerArray_lower_bound(IntegerArray *array, uint64_t item) {
    int low = 0;
    int high = array->size - 1;

    while (low <= high) {
        int m = (low + high) / 2;

        if (array->items[m] < item) {
            low = m + 1;
        } else if (array->items[m] > item) {
            high = m - 1;
        } else {
            return m;
        }
    }

    return low;
}

int IntegerArray_insert_sorted(IntegerArray *array, uint64_t item) {
    int index = IntegerArray_lower_bound(array, item);
    IntegerArray_insert_at_index(array, index, item);
    return index;
}

void IntegerArray_append(IntegerArray *array, uint64_t item) {
    array->items[array->size] = item;
    array->size++;
}

static int IntegerArray_find_index(IntegerArray *array, uint64_t item) {
    int low = 0;
    int high = array->size - 1;

    while (low <= high) {
        int m = (low + high) / 2;

        if (array->items[m] < item) {
            low = m + 1;
        } else if (array->items[m] > item) {
            high = m - 1;
        } else {
            return m;
        }
    }

    return -1;
}

bool IntegerArray_binary_search(IntegerArray *array, uint64_t item, int *index) {
    *index = IntegerArray_find_index(array, item);
    return *index != -1;
}

void IntegerArray_delete_at_index(IntegerArray *array, int index) {
    for (int i = index; i < array->size - 1; i++) {
        array->items[i] = array->items[i + 1];
    }

    array->size--;
}

void IntegerArray_print(IntegerArray *array) {
    printf("[");

    for (int i = 0; i < array->size; i++) {
        printf("%lu", array->items[i]);

        if (i < array->size - 1) {
            printf(", ");
        }
    }

    printf("]\n");
}

void IntegerArray_clear(IntegerArray *array) {
    array->size = 0;
}

void IntegerArray_copy(IntegerArray *src, IntegerArray *dest) {
    for (int i = 0; i < src->size; i++) {
        dest->items[i] = src->items[i];
    }

    dest->size = src->size;
}

// BPTreeNodeArray

BPTreeNodeArray *BPTreeNodeArray_init(int capacity) {
    BPTreeNodeArray *array = (BPTreeNodeArray *)malloc(sizeof(BPTreeNodeArray));
    array->items = (BPTreeNode **)malloc(sizeof(BPTreeNode *) * capacity);
    array->size = 0;
    return array;
}

void BPTreeNodeArray_destroy(BPTreeNodeArray **array) {
    free((*array)->items);
    free(*array);
    *array = NULL;
}

void BPTreeNodeArray_insert_at_index(BPTreeNodeArray *array, int index, BPTreeNode *item) {
    for (int i = array->size - 1; i >= index; i--) {
        array->items[i + 1] = array->items[i];
    }

    array->items[index] = item;
    array->size++;
}

void BPTreeNodeArray_append(BPTreeNodeArray *array, BPTreeNode *item) {
    array->items[array->size] = item;
    array->size++;
}

int BPTreeNodeArray_search(BPTreeNodeArray *array, BPTreeNode *item) {
    for (int i = 0; i < array->size; i++) {
        if (array->items[i] == item) {
            return i;
        }
    }

    return -1;
}

void BPTreeNodeArray_delete_at_index(BPTreeNodeArray *array, int index) {
    for (int i = index; i < array->size - 1; i++) {
        array->items[i] = array->items[i + 1];
    }

    array->size--;
}

void BPTreeNodeArray_clear(BPTreeNodeArray *array) {
    array->size = 0;
}

void BPTreeNodeArray_copy(BPTreeNodeArray *src, BPTreeNodeArray *dest) {
    for (int i = 0; i < src->size; i++) {
        dest->items[i] = src->items[i];
    }

    dest->size = src->size;
}

// ByteArray

ByteArray *ByteArray_init(int capacity) {
    ByteArray *array = (ByteArray *)malloc(sizeof(ByteArray));
    array->items = (uint8_t *)malloc(sizeof(uint8_t) * capacity);
    array->size = 0;
    return array;
}

void ByteArray_destroy(ByteArray **array) {
    free((*array)->items);
    free(*array);
    *array = NULL;
}

void ByteArray_append(ByteArray *array, uint8_t item) {
    array->items[array->size] = item;
    array->size++;
}
