/**
 * @file BPTreeTests.c
 * @author Florian Burgener (florian.burgener@etu.hesge.ch)
 * @version 1.0
 * @date 2022-06-17
 */
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "../Array.h"
#include "../BPTree.h"
#include "Unity/unity.h"

#define RANDOM_MIN 0
#define RANDOM_MAX 1000

void setUp() {
}

void tearDown() {
}

// BEGIN : Functions that check that a B+ tree is compliant with the B+ Tree rules.

/**
 * @brief Finds the leftmost leaf node.
 *
 * @param root The root node.
 * @return BPTreeNode* The leftmost leaf node.
 */
static BPTreeNode *find_first_leaf(BPTreeNode *root) {
    if (root->is_leaf) {
        return root;
    }

    return find_first_leaf(root->children->items[0]);
}

static bool check_if_the_leaf_nodes_are_correctly_chained(BPTreeNode *root) {
    BPTreeNode *current = find_first_leaf(root);

    while (current->next != NULL) {
        if (current->keys->items[current->keys->size - 1] >= current->next->keys->items[0]) {
            return false;
        }

        current = current->next;
    }

    return true;
}

static bool check_if_all_leaf_nodes_nodes_have_no_children(BPTreeNode *root) {
    if (root->is_leaf) {
        return root->children->size == 0;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_leaf_nodes_nodes_have_no_children(root->children->items[i])) {
            return false;
        }
    }

    return true;
}

static bool check_if_all_leaf_nodes_have_as_many_keys_as_data(BPTreeNode *root) {
    if (root->is_leaf) {
        return root->keys->size == root->data->size;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_leaf_nodes_have_as_many_keys_as_data(root->children->items[i])) {
            return false;
        }
    }

    return true;
}

static int compute_first_leaf_depth(BPTreeNode *root) {
    if (root->is_leaf) {
        return 0;
    }

    return compute_first_leaf_depth(root->children->items[0]) + 1;
}

static bool check_if_all_leaf_nodes_are_at_the_same_depth(BPTreeNode *root, int expected_depth, int depth) {
    if (root->is_leaf) {
        return depth == expected_depth;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_leaf_nodes_are_at_the_same_depth(root->children->items[i], expected_depth, depth + 1)) {
            return false;
        }
    }

    return true;
}

static bool check_if_all_internal_nodes_have_no_data(BPTreeNode *root) {
    if (root->is_leaf) {
        return true;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_internal_nodes_have_no_data(root->children->items[i])) {
            return false;
        }
    }

    return root->data->size == 0;
}

static bool check_if_all_internal_nodes_have_always_1_child_more_than_the_number_of_keys(BPTreeNode *root) {
    if (root->is_leaf) {
        return true;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_internal_nodes_have_always_1_child_more_than_the_number_of_keys(root->children->items[i])) {
            return false;
        }
    }

    return root->children->size == root->keys->size + 1;
}

static bool check_if_the_array_is_sorted(IntegerArray *array) {
    for (int i = 0; i < array->size - 1; i++) {
        if (array->items[i] > array->items[i + 1]) {
            return false;
        }
    }

    return true;
}

static bool check_if_all_nodes_have_their_keys_sorted(BPTreeNode *root) {
    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_nodes_have_their_keys_sorted(root->children->items[i])) {
            return false;
        }
    }

    return check_if_the_array_is_sorted(root->keys);
}

static bool check_if_all_nodes_have_no_more_keys_than_the_maximum_allowed(BPTreeNode *root) {
    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_nodes_have_no_more_keys_than_the_maximum_allowed(root->children->items[i])) {
            return false;
        }
    }

    return root->keys->size <= 2 * root->order;
}

static bool check_if_all_nodes_except_the_root_node_have_not_less_keys_than_the_minimum_required(BPTreeNode *root, BPTreeNode *parent) {
    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_all_nodes_except_the_root_node_have_not_less_keys_than_the_minimum_required(root->children->items[i], root)) {
            return false;
        }
    }

    if (parent == NULL) {
        return true;
    }

    return root->keys->size >= root->order;
}

static bool check_if_the_keys_have_been_correctly_inserted_within_range(BPTreeNode *root, uint64_t left, uint64_t right) {
    for (int i = 0; i < root->keys->size; i++) {
        if (root->keys->items[i] < left || root->keys->items[i] >= right) {
            return false;
        }
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_the_keys_have_been_correctly_inserted_within_range(root->children->items[i], left, right)) {
            return false;
        }
    }

    return true;
}

static bool check_if_the_keys_have_been_correctly_inserted(BPTreeNode *root) {
    if (root->is_leaf) {
        return true;
    }

    for (int i = 0; i < root->children->size; i++) {
        if (i == 0) {
            if (!check_if_the_keys_have_been_correctly_inserted_within_range(root->children->items[i], 0, root->keys->items[i])) {
                return false;
            }
        } else if (i == root->children->size - 1) {
            if (!check_if_the_keys_have_been_correctly_inserted_within_range(root->children->items[i], root->keys->items[i - 1], 18446744073709551615LLU)) {
                return false;
            }
        } else {
            if (!check_if_the_keys_have_been_correctly_inserted_within_range(root->children->items[i], root->keys->items[i - 1], root->keys->items[i])) {
                return false;
            }
        }
    }

    for (int i = 0; i < root->children->size; i++) {
        if (!check_if_the_keys_have_been_correctly_inserted(root->children->items[i])) {
            return false;
        }
    }

    return true;
}

static bool check_BPTree_compliance(BPTreeNode *root) {
    bool is_compliant = true;

    is_compliant &= check_if_the_leaf_nodes_are_correctly_chained(root);
    is_compliant &= check_if_all_leaf_nodes_nodes_have_no_children(root);
    is_compliant &= check_if_all_leaf_nodes_have_as_many_keys_as_data(root);
    is_compliant &= check_if_all_leaf_nodes_are_at_the_same_depth(root, compute_first_leaf_depth(root), 0);

    is_compliant &= check_if_all_internal_nodes_have_no_data(root);
    is_compliant &= check_if_all_internal_nodes_have_always_1_child_more_than_the_number_of_keys(root);

    is_compliant &= check_if_all_nodes_have_their_keys_sorted(root);
    is_compliant &= check_if_all_nodes_have_no_more_keys_than_the_maximum_allowed(root);
    is_compliant &= check_if_all_nodes_except_the_root_node_have_not_less_keys_than_the_minimum_required(root, NULL);

    is_compliant &= check_if_the_keys_have_been_correctly_inserted(root);

    return is_compliant;
}

// END : Functions that check that a B+ tree is compliant with the B+ Tree rules.

/**
 * @brief Checks if an item is present in the array.
 *
 * @param array The array in which the item is searched.
 * @param item The sought-after item.
 * @return true The item has been found.
 * @return false The item has not been found.
 */
static bool IntegerArray_search(IntegerArray *array, uint64_t item) {
    for (int i = 0; i < array->size; i++) {
        if (array->items[i] == item) {
            return true;
        }
    }

    return false;
}

/**
 * @brief Generates an array of random numbers.
 *
 * @param size The number of random numbers to generate.
 * @param random_min The smallest random value.
 * @param random_max The greatest random value.
 * @return IntegerArray* The generated array filled with random numbers.
 */
static IntegerArray *generate_random_numbers_array(int size, int random_min, int random_max) {
    IntegerArray *array = IntegerArray_init(size);

    for (int i = 0; i < size; i++) {
        uint64_t number;

        // While the generated random nomber already exists in the array.
        do {
            number = random_min + rand() % (random_max - random_min);
        } while (IntegerArray_search(array, number));

        IntegerArray_append(array, number);
    }

    return array;
}

/**
 * @brief Transforms a key into data, this function allows to make a kind of hash to check the relation between a key and a data.
 *
 * @param key The key to transform.
 * @return uint64_t The key transformed into data.
 */
static uint64_t transform_key_to_data(uint64_t key) {
    return key * 10000;
}

// BEGIN : Tests

// **** BEGIN : test_BPTree_insert

static void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(int order) {
    int size = 1;

    // Test 10 times, variable i being the seed.
    for (int i = 0; i < 10; i++) {
        srand(i);

        // Test with seed "i" 8 array size, at each end of loop the size variable is multiplied by 2, the array of random keys
        // will have the size of 1 then 2, 4, 8, 16, 32, ...
        for (int j = 0; j < 8; j++) {
            IntegerArray *keys = generate_random_numbers_array(size, RANDOM_MIN, RANDOM_MAX);
            BPTreeNode *root = BPTree_init(order);

            for (int i = 0; i < keys->size; i++) {
                BPTree_insert(root, keys->items[i], transform_key_to_data(keys->items[i]));
                // After each insertion, is checked that the B+ tree conforms to the rules of the B+ tree.
                TEST_ASSERT(check_BPTree_compliance(root));
            }

            IntegerArray_destroy(&keys);
            BPTree_destroy(&root);

            size *= 2;
        }

        size = 1;
    }
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_1() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(1);
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_2() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(2);
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_3() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(3);
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_4() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(4);
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_8() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(8);
}

void test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_16() {
    test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_given_order(16);
}

// **** END : test_BPTree_insert

// **** BEGIN : test_BPTree_delete

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(int order) {
    int size = 1;

    // Test 10 times, variable i being the seed.
    for (int i = 0; i < 1; i++) {
        srand(i);

        // Test with seed "i" 8 array size, at each end of loop the size variable is multiplied by 2, the array of random keys
        // will have the size of 1 then 2, 4, 8, 16, 32, ...
        for (int j = 0; j < 3; j++) {
            IntegerArray *keys = generate_random_numbers_array(size, RANDOM_MIN, RANDOM_MAX);
            BPTreeNode *root = BPTree_init(order);

            for (int i = 0; i < keys->size; i++) {
                BPTree_insert(root, keys->items[i], transform_key_to_data(keys->items[i]));
            }

            while (keys->size > 0) {
                int index = rand() % keys->size;
                BPTree_delete(root, keys->items[index]);
                // After each deletion, is checked that the B+ tree conforms to the rules of the B+ tree.
                TEST_ASSERT(check_BPTree_compliance(root));
                IntegerArray_delete_at_index(keys, index);
            }

            IntegerArray_destroy(&keys);
            BPTree_destroy(&root);

            size *= 2;
        }

        size = 1;
    }
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_1() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(1);
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_2() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(2);
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_3() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(3);
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_4() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(4);
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_8() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(8);
}

void test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_16() {
    test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_given_order(16);
}

// **** END : test_BPTree_delete

// **** BEGIN : test_BPTree_search

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(int order) {
    int size = 256;

    // Test 10 times, variable i being the seed.
    for (int i = 0; i < 1; i++) {
        srand(i);

        IntegerArray *keys = generate_random_numbers_array(size, RANDOM_MIN, RANDOM_MAX);
        BPTreeNode *root = BPTree_init(order);

        for (int i = 0; i < keys->size; i++) {
            BPTree_insert(root, keys->items[i], transform_key_to_data(keys->items[i]));
        }

        for (int i = 0; i < keys->size; i++) {
            uint64_t data;
            bool is_found = BPTree_search(root, keys->items[i], &data);

            // After each search, is checked that the key has been found and that the relationship between it and the data is correct.
            TEST_ASSERT(is_found);
            TEST_ASSERT_EQUAL_UINT64(transform_key_to_data(keys->items[i]), data);
        }

        IntegerArray_destroy(&keys);
        BPTree_destroy(&root);
    }
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_1() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(1);
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_2() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(2);
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_3() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(3);
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_4() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(4);
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_8() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(8);
}

void test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_16() {
    test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_given_order(16);
}

// **** END : test_BPTree_search

// END : Tests

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_1);
    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_2);
    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_3);
    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_4);
    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_8);
    RUN_TEST(test_BPTree_insert_should_comply_with_BPTree_rules_using_BPTree_of_order_16);

    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_1);
    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_2);
    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_3);
    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_4);
    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_8);
    RUN_TEST(test_BPTree_delete_should_comply_with_BPTree_rules_using_BPTree_of_order_16);

    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_1);
    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_2);
    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_3);
    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_4);
    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_8);
    RUN_TEST(test_BPTree_search_should_find_the_keys_that_exist_in_the_BPTree_using_BPTree_of_order_16);

    return UNITY_END();
}
