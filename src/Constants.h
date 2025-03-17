#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <GL/glew.h>

namespace Constants {
    constexpr int BOARD_SIZE = 24;
    constexpr int WINDOW_HEIGHT = 900;
    //constexpr int WINDOW_HEIGHT = 45 * BOARD_SIZE;
    constexpr int WINDOW_WIDTH = WINDOW_HEIGHT;
    constexpr double QUAD_SIZE = ((double)WINDOW_HEIGHT - (double)BOARD_SIZE) / (double)BOARD_SIZE;
    constexpr double IQUAD_SIZE = (double)BOARD_SIZE / ((double)WINDOW_HEIGHT - (double)BOARD_SIZE);
}

#endif // CONSTANTS_HPP
