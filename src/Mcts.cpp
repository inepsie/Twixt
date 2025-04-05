#include "Mcts.h"
#include "Constants.h"

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

void Mcts::init(){
  Node racine;
  std::array<GLuint, C::BOARD_SIZE_2> pawns = {};
  std::array<GLuint, C::BOARD_SIZE_2> links = {};
  m_tree.reserve(100000); // Réserve 10 000 emplacements pour éviter les
                          // réallocations fréquentes
  add_node(pawns, links);
}

void Mcts::add_node(std::array<GLuint, C::BOARD_SIZE_2> arg_pawns, std::array<GLuint, C::BOARD_SIZE_2> arg_links) {
    Node new_node;
    m_tree.push_back(new_node);
}

void Mcts::select() {}
void Mcts::expand() {}
void Mcts::best_move() {}

/*
std::array<GLuint, C::BOARD_SIZE_2> Mcts::apply_move(size_t i, size_t j){
    return m_
}
*/
