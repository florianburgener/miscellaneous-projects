/**
 * @file Point.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "Point.h"

#include <math.h>

#include "Rectangle.h"
#include "Vector2.h"

Point point_init(int x, int y) {
    return (Point){.x = x, .y = y};
}

Point position_to_point(Vector2 position, Rectangle *universe, int width, int height) {
    double dx = universe->bottom_right.x - universe->top_left.x;
    double dy = universe->bottom_right.y - universe->top_left.y;
    int x = (int)round(width * (position.x - universe->top_left.x) / dx);
    int y = (int)round(height * (position.y - universe->top_left.y) / dy);
    return point_init(x, y);
}
