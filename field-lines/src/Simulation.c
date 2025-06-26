/**
 * @file Simulation.c
 * @version 1.0
 * @date 2022-05-09
 */

#include "Simulation.h"

#include <math.h>
#include <stdlib.h>

#include "Charge.h"
#include "Graphics.h"
#include "Vector2.h"
#include "constants.h"
#include "random_number.h"

/**
 * @brief Checks if the point is outside the limits.
 *
 * @param universe The universe rectangle.
 * @param point The point
 * @return true Is outside the limits.
 * @return false Is not outside the limits.
 */
static bool is_out_of_bounds(Rectangle *universe, Vector2 point);

/**
 * @brief Generates a random charge.
 *
 * @return Charge The random charge.
 */
static Charge generate_random_charge() {
    Vector2 position = vector2_init(random_number_between_0_and_1(), random_number_between_0_and_1());
    int sign = rand() % 2 == 0 ? 1 : -1;
    return charge_init(ELEMENTARY_CHARGE * sign, position);
}

/**
 * @brief Generates random charges.
 *
 * @param universe The universe rectangle.
 * @param charges_length The number of charges.
 * @return Charge* The charge array.
 */
#include <stdio.h>
static Charge *generate_random_charges(Rectangle *universe, int *charges_length) {
    *charges_length = random_number_between(MIN_CHARGES, MAX_CHARGES);
    Charge *charges = (Charge *)malloc(sizeof(Charge) * *charges_length);

    for (int i = 0; i < *charges_length; i++) {
        while (true) {
            Charge charge = generate_random_charge();

            // Checks if the charge is not too close to the edge of the universe.
            Vector2 position = charge.position;
            double eps = RADIUS_TRESHOLD;
            if (is_out_of_bounds(universe, vector2_add(position, vector2_init(0, -eps)))) {
                // Starts the loop again with a new random charge.
                continue;
            }
            if (is_out_of_bounds(universe, vector2_add(position, vector2_init(eps, 0)))) {
                continue;
            }
            if (is_out_of_bounds(universe, vector2_add(position, vector2_init(0, eps)))) {
                continue;
            }
            if (is_out_of_bounds(universe, vector2_add(position, vector2_init(-eps, 0)))) {
                continue;
            }

            bool charges_are_too_close = false;
            for (int j = 0; j < i; j++) {
                double norm = vector2_norm(vector2_substract(position, charges[j].position));
                if (norm < RADIUS_TRESHOLD * 3) {
                    // The two charges are too close.
                    charges_are_too_close = true;
                }
            }

            if (charges_are_too_close) {
                // Starts the loop again with a new random charge.
                continue;
            }

            charges[i] = charge;
            break;
        }
    }

    return charges;
}

/**
 * @brief Draws the charges.
 *
 * @param simulation The simulation.
 * @param graphics The graphics in which we draw.
 */
static void draw_charges(Simulation *simulation, Graphics *graphics) {
    for (int i = 0; i < simulation->charges_length; i++) {
        charge_draw(simulation->charges[i], graphics, simulation->universe);
    }
}

/**
 * @brief Calculates the electric field.
 *
 * @param charge The charge.
 * @param point The point
 * @param eps Minimum distance between a point and the position of a charge.
 * @param e The electric field.
 * @return true The electric field is valid.
 * @return false The electric field is invalid.
 */
static bool compute_e(Charge charge, Vector2 point, double eps, Vector2 *e) {
    Vector2 r = vector2_substract(charge.position, point);
    double e_intensity = K * fabs(charge.q) / vector2_norm_sqr(r);
    *e = vector2_multiply(vector2_normalize(r), e_intensity);

    if (charge.q > 0) {
        *e = vector2_multiply(*e, -1);
    }

    return vector2_norm(r) >= eps;
}

/**
 * @brief Calculates the electric field at the given point.
 *
 * @param charges_length The number of charges.
 * @param charges The charge array.
 * @param point The point.
 * @param eps Minimum distance between a point and the position of a charge.
 * @param e The electric field.
 * @return true The electric field is valid.
 * @return false The electric field is invalid.
 */
static bool compute_total_normalized_e(int charges_length, Charge *charges, Vector2 point, double eps, Vector2 *e) {
    *e = vector2_init_zero();

    for (int i = 0; i < charges_length; i++) {
        Vector2 e_i;
        if (!compute_e(charges[i], point, eps, &e_i)) {
            return false;
        }

        *e = vector2_add(*e, e_i);
    }

    *e = vector2_normalize(*e);
    return true;
}

static bool is_out_of_bounds(Rectangle *universe, Vector2 point) {
    return point.x < universe->top_left.x || point.x > universe->bottom_right.x || point.y < universe->top_left.y || point.y > universe->bottom_right.y;
}

/**
 * @brief Calculates the next point.
 *
 * @param simulation The simulation.
 * @param direction The direction in which we draw.
 * @param current_point The point from which the next point is calculated.
 * @param eps Minimum distance between a point and the position of a charge.
 * @param next_point The next point.
 * @return true The point is valid.
 * @return false The point is invalid.
 */
static bool compute_next_point(Simulation *simulation, int direction, Vector2 current_point, double eps, Vector2 *next_point) {
    Vector2 electric_field_normalized;
    if (!compute_total_normalized_e(simulation->charges_length, simulation->charges, current_point, eps, &electric_field_normalized)) {
        return false;
    }

    *next_point = vector2_multiply(vector2_multiply(electric_field_normalized, simulation->delta_x), direction);
    *next_point = vector2_add(current_point, *next_point);

    if (is_out_of_bounds(simulation->universe, *next_point)) {
        return false;
    }

    return true;
}

/**
 * @brief Draws the field line in one direction.
 *
 * @param simulation The simulation.
 * @param graphics The graphics in which we draw.
 * @param starting_point The starting point.
 * @param direction The direction in which we draw.
 */
static void draw_field_line_with_direction(Simulation *simulation, Graphics *graphics, Vector2 starting_point, int direction) {
    Vector2 current_point = starting_point;

    while (true) {
        Vector2 next_point;
        if (!compute_next_point(simulation, direction, current_point, RADIUS_TRESHOLD, &next_point)) {
            break;
        }

        Point p0 = position_to_point(current_point, simulation->universe, SCREEN_WIDTH, SCREEN_HEIGHT);
        Point p1 = position_to_point(next_point, simulation->universe, SCREEN_WIDTH, SCREEN_HEIGHT);
        gfx_draw_line(graphics, p0, p1, COLOR_WHITE);
        current_point = next_point;
    }
}

/**
 * @brief Draws the field line.
 *
 * @param simulation The simulation.
 * @param graphics The graphics in which we draw.
 * @param starting_point The starting point.
 */
static void draw_field_line(Simulation *simulation, Graphics *graphics, Vector2 starting_point) {
    draw_field_line_with_direction(simulation, graphics, starting_point, -1);
    draw_field_line_with_direction(simulation, graphics, starting_point, 1);
}

Simulation *simulation_init(Rectangle *universe, double delta_x) {
    Simulation *simulation = (Simulation *)malloc(sizeof(Simulation));
    simulation->universe = universe;
    simulation->charges = generate_random_charges(simulation->universe, &simulation->charges_length);
    simulation->delta_x = delta_x;
    return simulation;
}

void simulation_destroy(Simulation **simulation) {
    free((*simulation)->charges);
    free(*simulation);
    *simulation = NULL;
}

void simulation_draw(Simulation *simulation, Graphics *graphics) {
    draw_charges(simulation, graphics);

    // // Drawing of field lines from randomly placed points.
    // int number_of_random_points = 100;
    // for (int i = 0; i < number_of_random_points; i++) {
    //     Vector2 starting_point = vector2_init(random_number_between_0_and_1(), random_number_between_0_and_1());
    //     draw_field_line(simulation, graphics, starting_point);
    // }

    // Drawing of the field lines from points placed around each of the particles (the display is more homogeneous with this technique).
    for (int32_t i = 0; i < simulation->charges_length; i++) {
        Vector2 position = simulation->charges[i].position;

        double angle = 0;
        while (angle < 2 * M_PI) {
            angle += 2 * M_PI / 64;
            double x = cos(angle) * RADIUS_TRESHOLD * 1.1;
            double y = sin(angle) * RADIUS_TRESHOLD * 1.1;
            Vector2 starting_point = vector2_add(position, vector2_init(x, y));
            draw_field_line(simulation, graphics, starting_point);
        }
    }
}
