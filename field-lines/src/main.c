/**
 * @file main.c
 * @version 1.0
 * @date 2022-05-09
 */

#include <math.h>
#include <stdlib.h>
#include <time.h>

#include "Graphics.h"
#include "Rectangle.h"
#include "Simulation.h"
#include "constants.h"

// https://stackoverflow.com/questions/3417837/what-is-the-best-way-to-suppress-a-unused-variable-x-warning
#ifdef UNUSED
#elif defined(__GNUC__)
#define UNUSED(x) UNUSED_##x __attribute__((unused))
#elif defined(__LCLINT__)
#define UNUSED(x) /*@unused@*/ x
#else
#define UNUSED(x) x
#endif

/**
 * @brief Calculates the delta x used to draw the lines.
 *
 * @param width The width of the screen.
 * @param height The height of the screen.
 * @return double The delta x.
 */
double compute_delta_x(int width, int height) {
    return 1 / sqrt((width * width) + (height * height));
}

int main(int UNUSED(argc), char *UNUSED(argv[])) {
    Graphics *graphics = gfx_create("Field Lines Simulation", SCREEN_WIDTH, SCREEN_HEIGHT);
    srand(time(NULL));

    if (graphics == NULL) {
        fprintf(stderr, "Impossible to initialize the window.\n");
        return EXIT_FAILURE;
    }

    gfx_clear(graphics, COLOR_BLACK);
    Rectangle *universe = rectangle_init(UNIVERSE_X0, UNIVERSE_Y0, UNIVERSE_X1, UNIVERSE_Y1);
    Simulation *simulation = simulation_init(universe, compute_delta_x(SCREEN_WIDTH, SCREEN_HEIGHT));
    simulation_draw(simulation, graphics);
    simulation_destroy(&simulation);
    rectangle_destroy(&universe);
    gfx_present(graphics);

    while (true) {
        if (gfx_keypressed() == SDLK_ESCAPE) {
            break;
        }
    }

    gfx_destroy(graphics);
    return EXIT_SUCCESS;
}
