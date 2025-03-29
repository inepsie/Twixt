#ifndef BOARD_H_
#define BOARD_H_
#include "Camera.h"
#include "Constants.h"
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

namespace C = Constants;

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
  void print_links();
  void init(size_t size);
  void add_edge_lines();
  void init_lines();
  size_t id_2dto1d(size_t i, size_t j);
  std::array<size_t, 2> id_1dto2d(size_t ind);
  void draw(Camera cam);
  void draw_lines(Camera cam);
  void add_line(size_t i, size_t j, size_t ni, size_t nj);
  void draw_points(Camera cam);
  void reset(GLuint valeur);
  int win();
  void add_win_pawn(size_t i, size_t j);
  void print_win_pawn();
  void play(size_t i, size_t j);
  std::array<int, 2> rot90ind(size_t nb_rot, int i, int j);
  void block_link(size_t type, size_t i, size_t j);
  void check_links(size_t i, size_t j);
  void add_link(size_t type, size_t i, size_t j, size_t ni, size_t nj);
  size_t reverse_link(size_t type);
  bool unbound(size_t ind);
  bool unbound(size_t i, size_t j);
        size_t get_player();
        bool ended = 0;
  size_t link_ind_1D(size_t type, size_t ind);
  std::array<size_t, 2> link_ind(size_t type, size_t i, size_t j);

private:
  size_t m_size = C::BOARD_SIZE;
  size_t m_size_2 = C::BOARD_SIZE_2;
  std::array<GLuint, C::BOARD_SIZE_2> m_board =
      {}; // contient l'état des pions, pas les links
  std::array<std::array<GLuint, 8>, C::BOARD_SIZE_2> m_links =
      {}; // contient l'état des connexions
  size_t m_turn = 0;
  size_t m_player = 0;
  GLuint m_pointVAO, m_buffer, m_lineVBO, m_lineVAO, m_colorVBO;
  std::vector<glm::vec3> m_lines;       // Liste des connexions
  std::vector<glm::vec3> m_linesColors; // Stocke les couleurs des lignes
  std::vector<GLuint> m_top;            //
  std::vector<GLuint> m_bot;            //
  std::vector<GLuint> m_left;           //
  std::vector<GLuint> m_right;          //
};

#endif
