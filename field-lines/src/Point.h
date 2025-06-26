/**
 * @file Point.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef POINT_H
#define POINT_H

#include "Rectangle.h"
#include "Vector2.h"

typedef struct Point {
    int x;
    int y;
} Point;

/**
 * @brief Initializes a Point.
 *
 * @param x The x component of the point.
 * @param y The y component of the point.
 * @return Point The point.
 */
Point point_init(int x, int y);

/**
 * @brief Converts a position to a point in a rectangle of size width x height.
 *
 * @param position The position in the universe.
 * @param universe The universe rectangle.
 * @param width The width of the screen.
 * @param height The height of the screen.
 * @return Point The point located on the screen.
 */
Point position_to_point(Vector2 position, Rectangle *universe, int width, int height);

#endif
