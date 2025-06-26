/**
 * @file constants.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "constants.h"

const int SCREEN_WIDTH = 750;
const int SCREEN_HEIGHT = 750;
const int UNIVERSE_X0 = 0;
const int UNIVERSE_Y0 = 0;
const int UNIVERSE_X1 = 1;
const int UNIVERSE_Y1 = 1;
const int CHARGE_CIRCLE_RADIUS = 20;
const int MIN_CHARGES = 2;
const int MAX_CHARGES = 5;
const double K = 8.988e9;
const double ELEMENTARY_CHARGE = 1.602e-19;
// Represents the acceptable distance between the last point and the charge, this distance is calculated with
// the radius of the charge display circle and the width of the window.
const double RADIUS_TRESHOLD = (double)CHARGE_CIRCLE_RADIUS / SCREEN_WIDTH;
