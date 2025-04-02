#ifndef NODE_H_
#define NODE_H_

#include "Constants.h"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

namespace C = Constants;

class Node {
public:
    Node();
    ~Node();
  std::array<GLuint, C::BOARD_SIZE_2> apply_move();
  size_t nb_played = 0;
  size_t nb_win = 0;
    int player = 0;

private:
  std::array<GLuint, C::BOARD_SIZE_2> pawns;
  std::array<GLuint, C::BOARD_SIZE_2> links;
  std::vector<size_t> child;
};

#endif
