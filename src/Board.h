#ifndef BOARD_H_
#define BOARD_H_

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Board {
public:
    Board(size_t size);
    ~Board();

private:
    size_t m_size;
    std::vector<GLuint> m_board;
    std::vector<GLuint> m_links;
};

#endif // BOARD_H_
