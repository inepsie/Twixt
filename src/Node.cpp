#include "Node.h"
#include "Constants.h"
#include "Utils.h"

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

Node::Node() {
  std::array<GLuint, C::BOARD_SIZE_2> pawns = {};
  std::array<GLuint, C::BOARD_SIZE_2> links = {};
  //m_tree.reserve(100000); // Réserve 10 000 emplacements pour éviter les
                          // réallocations fréquentes
  //add_node(pawns, links);
}

// Définition du destructeur
Node::~Node() {}

void Node::apply_move(std::array<GLuint, C::BOARD_SIZE_2>, size_t i, size_t j){
}

void Board::play(size_t i, size_t j) {
  size_t ind = id_2dto1d(i, j);
  size_t screen_ind = id_2dto1d(C::BOARD_SIZE - 1 - i, C::BOARD_SIZE - 1 - j);
  size_t offset = screen_ind * sizeof(GLuint);
  size_t size = m_size_2 * sizeof(GLuint);
  GLuint *ptr = nullptr;
  GLuint val = 2 + (m_turn % 2);
  if (unbound(i, j)) // En dehors du board
    return;
  if (m_board[ind] != 1) // Deja pris
    return;
  if ((i==0 || i==(C::BOARD_SIZE-1)) && (m_turn%2)==0) return; // Bord de l'autre joueur
  if ((j==0 || j==(C::BOARD_SIZE-1)) && (m_turn%2)==1) return; // Bord de l'autre joueur
  m_board[ind] = val;

  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  ptr = (GLuint *)glMapBufferRange(GL_ARRAY_BUFFER, offset, sizeof(GLuint),
                                   GL_MAP_WRITE_BIT |
                                       GL_MAP_INVALIDATE_RANGE_BIT);
  if (ptr) {
    memcpy(ptr, &val, sizeof(GLuint)); // Écriture en mémoire GPU
    glUnmapBuffer(GL_ARRAY_BUFFER);    // Fin du mapping
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  check_links(i, j);

  add_win_pawn(i, j);
  //print_win_pawn();
  win();
  m_turn += 1;
  m_player = (m_player + 1) % 2;
}
