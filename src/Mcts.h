#ifndef MCTS_H_
#define MCTS_H_

#include "Node.h"
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

struct Node {
  std::array<GLuint, C::BOARD_SIZE_2> pawns;
  std::array<GLuint, C::BOARD_SIZE_2> links;
  std::vector<size_t> child;
  //child.reserve(10); // Réserve 10 000 emplacements pour éviter les réallocations fréquentes
  size_t nb_played = 0;
  size_t nb_win = 0;
    int player = 0;
};

class Mcts {
public:
    Mcts();
    ~Mcts();
  //void add_node();
void add_node(std::array<GLuint, C::BOARD_SIZE_2> arg_pawns, std::array<GLuint, C::BOARD_SIZE_2> arg_links);
  void select();
  void expand();
  void best_move();
  std::array<GLuint, C::BOARD_SIZE_2> apply_move();

private:
  std::vector<Node> m_tree;
};

#endif // MCTS_H_
