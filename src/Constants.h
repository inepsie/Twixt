#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <GL/glew.h>

namespace Constants {
    constexpr int BOARD_SIZE = 24;
    constexpr int WINDOW_HEIGHT = 900;
    //constexpr int WINDOW_HEIGHT = 45 * BOARD_SIZE;
    constexpr int WINDOW_WIDTH = WINDOW_HEIGHT;
    constexpr double QUAD_SIZE = ((double)WINDOW_WIDTH - (double)BOARD_SIZE) / (double)BOARD_SIZE;
    constexpr double IQUAD_SIZE = (double)BOARD_SIZE / ((double)WINDOW_WIDTH - (double)BOARD_SIZE);

    constexpr double FQUAD_SIZE = (((double)WINDOW_WIDTH) / (1.0 + (double)BOARD_SIZE)) / WINDOW_WIDTH;
}

#endif // CONSTANTS_HPP
