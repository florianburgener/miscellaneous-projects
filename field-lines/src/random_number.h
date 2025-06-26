/**
 * @file random_number.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef RANDOM_NUMBER_H
#define RANDOM_NUMBER_H

/**
 * @brief Draws a random number between min and max.
 *
 * @param min The minimum value.
 * @param max The maximum value.
 * @return int The random number.
 */
int random_number_between(int min, int max);

/**
 * @brief Draws a random number between 0 and 1.
 *
 * @return double The random number.
 */
double random_number_between_0_and_1();

#endif
