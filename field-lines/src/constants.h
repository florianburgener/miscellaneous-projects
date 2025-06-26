/**
 * @file constants.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

// Screen width in pixels.
extern const int SCREEN_WIDTH;
// Screen height in pixels.
extern const int SCREEN_HEIGHT;
// The x-component of the universe at the top left.
extern const int UNIVERSE_X0;
// The y-component of the universe at the top left.
extern const int UNIVERSE_Y0;
// The x-component of the universe at the bottom right.
extern const int UNIVERSE_X1;
// The y-component of the universe at the bottom right.
extern const int UNIVERSE_Y1;
// Radius of the circle of a charge.
extern const int CHARGE_CIRCLE_RADIUS;
// Minimum number of charges to generate.
extern const int MIN_CHARGES;
// Maximum number of charges to generate.
extern const int MAX_CHARGES;
// Coulomb's constant.
extern const double K;
// Value of the elementary charge.
extern const double ELEMENTARY_CHARGE;
// Minimum distance between a point and the position of a charge.
extern const double RADIUS_TRESHOLD;

#endif
