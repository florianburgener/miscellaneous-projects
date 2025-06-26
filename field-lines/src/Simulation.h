/**
 * @file Simulation.h
 * @version 1.0
 * @date 2022-05-09
 */

#ifndef SIMULATION_H
#define SIMULATION_H

#include "Charge.h"
#include "Graphics.h"
#include "Rectangle.h"

typedef struct Simulation {
    Rectangle *universe;
    int charges_length;
    Charge *charges;
    double delta_x;
} Simulation;

/**
 * @brief Initializes a Simulation.
 *
 * @param universe The universe rectangle.
 * @param delta_x The The delta x used for drawing lines.
 * @return Simulation* The simulation.
 */
Simulation *simulation_init(Rectangle *universe, double delta_x);

/**
 * @brief Frees memory of the simulation.
 *
 * @param simulation The simulation.
 */
void simulation_destroy(Simulation **simulation);

/**
 * @brief Draws the simulation.
 *
 * @param simulation The simulation.
 * @param graphics The graphics in which we draw.
 */
void simulation_draw(Simulation *simulation, Graphics *graphics);

#endif
