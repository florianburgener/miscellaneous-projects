/**
 * @file Vector2.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef VECTOR2_H
#define VECTOR2_H

typedef struct Vector2 {
    double x;
    double y;
} Vector2;

/**
 * @brief Initializes a Vector2.
 *
 * @param x The x component of the vector.
 * @param y The y component of the vector.
 * @return Vector2 The vector.
 */
Vector2 vector2_init(double x, double y);

/**
 * @brief Initializes a vector with the x and y component at 0.
 *
 * @return Vector2 The vector.
 */
Vector2 vector2_init_zero();

/**
 * @brief Adds two vectors.
 *
 * @param a The left vector.
 * @param b The right vector.
 * @return Vector2 The sum of the two vectors.
 */
Vector2 vector2_add(Vector2 a, Vector2 b);

/**
 * @brief Subtracts two vectors.
 *
 * @param a The left vector.
 * @param b The right vector.
 * @return The difference of the two vectors.
 */
Vector2 vector2_substract(Vector2 a, Vector2 b);

/**
 * @brief Multiplies a vector and a scalar.
 *
 * @param v The vector.
 * @param scalar The scalar.
 * @return Vector2 The result of the multiplication.
 */
Vector2 vector2_multiply(Vector2 v, double scalar);

/**
 * @brief Calculates the dot product between two vectors.
 *
 * @param a The left vector.
 * @param b The right vector.
 * @return double The result of the dot product.
 */
double vector2_dot_product(Vector2 a, Vector2 b);

/**
 * @brief Calculates the norm squared of the vector.
 *
 * @param v The vector.
 * @return double The norm squared.
 */
double vector2_norm_sqr(Vector2 v);

/**
 * @brief Calculates the norm of the vector.
 *
 * @param v The vector.
 * @return double The norm.
 */
double vector2_norm(Vector2 v);

/**
 * @brief Normalizes the vector.
 *
 * @param v The vector.
 * @return Vector2 The normalized vector.
 */
Vector2 vector2_normalize(Vector2 v);

#endif
