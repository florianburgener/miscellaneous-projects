/**
 * @file Rectangle.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "Rectangle.h"

#include <stdlib.h>

#include "Vector2.h"

Rectangle *rectangle_init(int x0, int y0, int x1, int y1) {
    Rectangle *rectangle = (Rectangle *)malloc(sizeof(Rectangle));
    rectangle->top_left = vector2_init(x0, y0);
    rectangle->bottom_right = vector2_init(x1, y1);
    return rectangle;
}

void rectangle_destroy(Rectangle **rectangle) {
    free(*rectangle);
    *rectangle = NULL;
}
