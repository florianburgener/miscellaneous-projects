/**
 * @file Charge.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "Charge.h"

#include "Graphics.h"
#include "Point.h"
#include "Rectangle.h"
#include "Vector2.h"
#include "constants.h"

Charge charge_init(double q, Vector2 position) {
    return (Charge){.q = q, .position = position};
}

void charge_draw(Charge charge, Graphics *graphics, Rectangle *universe) {
    Point center = position_to_point(charge.position, universe, SCREEN_WIDTH, SCREEN_HEIGHT);

    int radius = CHARGE_CIRCLE_RADIUS;
    gfx_draw_circle(graphics, center, radius, COLOR_WHITE);

    int color = charge.q > 0 ? COLOR_RED : COLOR_BLUE;
    int half_length = (int)(radius * .6);
    gfx_draw_line(graphics, point_init(center.x - half_length, center.y), point_init(center.x + half_length, center.y), color);

    if (charge.q > 0) {
        gfx_draw_line(graphics, point_init(center.x, center.y - half_length), point_init(center.x, center.y + half_length), color);
    }
}
