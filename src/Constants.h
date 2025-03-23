#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include <cstdint>
#include <GL/glew.h>

namespace Constants {
    constexpr int BOARD_SIZE = 12;
    constexpr int BOARD_SIZE_2 = BOARD_SIZE * BOARD_SIZE;
    constexpr int WINDOW_HEIGHT = 900;
    constexpr int WINDOW_WIDTH = WINDOW_HEIGHT;
    constexpr double NORM_QUAD_SIZE = 1.0 / (1.0 + (double)BOARD_SIZE);//size quad normalisé [0.0, 1.0]
    constexpr double QUAD_SIZE = ((double)WINDOW_WIDTH - (double)BOARD_SIZE) / (double)BOARD_SIZE;//non normalisé
}

#endif // CONSTANTS_HPP
