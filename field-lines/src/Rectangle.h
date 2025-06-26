/**
 * @file Rectangle.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "Vector2.h"

typedef struct Rectangle {
    Vector2 top_left;
    Vector2 bottom_right;
} Rectangle;

/**
 * @brief Initializes a Rectangle.
 *
 * @param x0 The x-component of the point at the top left.
 * @param y0 The y-component of the point at the top left.
 * @param x1 The x-component of the point at the bottom right.
 * @param y1 The y-component of the point at the bottom right.
 * @return Rectangle* The rectangle.
 */
Rectangle *rectangle_init(int x0, int y0, int x1, int y1);

/**
 * @brief Frees memory of the rectangle.
 *
 * @param rectangle
 */
void rectangle_destroy(Rectangle **rectangle);

#endif
