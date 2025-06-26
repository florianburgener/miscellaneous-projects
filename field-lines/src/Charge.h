/**
 * @file Charge.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef CHARGE_H
#define CHARGE_H

#include "Graphics.h"
#include "Rectangle.h"
#include "Vector2.h"

typedef struct Charge {
    double q;
    Vector2 position;
} Charge;

/**
 * @brief Initializes a Charge.
 *
 * @param q The electrical charge.
 * @param position The position of the charge in the universe.
 * @return Charge The charge.
 */
Charge charge_init(double q, Vector2 position);

/**
 * @brief Draws the charge.
 *
 * @param charge The charge.
 * @param graphics The graphics in which we draw.
 * @param universe The universe rectangle.
 */
void charge_draw(Charge charge, Graphics *graphics, Rectangle *universe);

#endif
