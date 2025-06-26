/**
 * @file Vector2.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "Vector2.h"

#include <math.h>

Vector2 vector2_init(double x, double y) {
    return (Vector2){.x = x, .y = y};
}

Vector2 vector2_init_zero() {
    return vector2_init(0, 0);
}

Vector2 vector2_add(Vector2 a, Vector2 b) {
    return vector2_init(a.x + b.x, a.y + b.y);
}

Vector2 vector2_substract(Vector2 a, Vector2 b) {
    return vector2_init(a.x - b.x, a.y - b.y);
}

Vector2 vector2_multiply(Vector2 v, double scalar) {
    return vector2_init(v.x * scalar, v.y * scalar);
}

double vector2_dot_product(Vector2 a, Vector2 b) {
    return a.x * b.x + a.y * b.y;
}

double vector2_norm_sqr(Vector2 v) {
    return vector2_dot_product(v, v);
}

double vector2_norm(Vector2 v) {
    return sqrt(vector2_norm_sqr(v));
}

Vector2 vector2_normalize(Vector2 v) {
    double norm = vector2_norm(v);
    return vector2_init(v.x / norm, v.y / norm);
}
