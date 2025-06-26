/**
 * @file random_number.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "random_number.h"

#include <stdlib.h>

int random_number_between(int min, int max) {
    return min + rand() % (max - min + 1);
}

double random_number_between_0_and_1() {
    return (double)rand() / RAND_MAX;
}
