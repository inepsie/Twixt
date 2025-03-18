#ifndef BOARD_H_
#define BOARD_H_
#include "Camera.h"
#include "Link.h"
#include <cstddef> // Pour size_t
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>

#define GLEW_STATIC
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class Board {
public:
  Board();  // Rendre le constructeur public
  ~Board(); // Rendre le destructeur public
  static std::unique_ptr<Board>
  createBoard(); // Méthode statique pour la création d'un Board
  static std::unique_ptr<Board> board;
  static Board &getInstance();
  static void
  reset(); // Permet de supprimer l'instance et d'en créer une nouvelle
        void print_board();
  void init(size_t size);
  size_t id_2dto1d(size_t i, size_t j);
  std::array<size_t, 2> id_1dto2d(size_t ind);
  void draw(Camera cam);
  void reset(GLuint valeur);
  void play(size_t i, size_t j);
  void check_links(size_t i, size_t j);

size_t reverse_ind_link(size_t type);
        void add_link(size_t type, size_t i, size_t j, size_t ni, size_t nj);
  bool unbound(size_t ind);
  bool unbound(size_t i, size_t j);
  std::array<size_t, 2> link_ind(size_t type, size_t i, size_t j);

private:
  std::vector<GLuint> m_board; // contient l'état des pions, pas les links
  std::vector<Link> m_links;
  size_t m_size = 0;
  size_t m_size_2 = 0;
  size_t m_turn = 0;
  size_t m_player = 0;
  GLuint m_vao, m_buffer;
};

#endif
