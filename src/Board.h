#ifndef BOARD_H_
#define BOARD_H_
#include "Camera.h"
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
  void init(size_t size);
  size_t id_2dto1d(size_t j, size_t i);
  std::array<size_t, 2> id_1dto2d(size_t ind);
  void play(size_t j, size_t i);

private:
  void draw(Camera cam);
  std::vector<GLuint> m_board; // contient l'état des pions, pas les links
  size_t m_size = 0;
  GLuint m_vao, m_buffer;
};

#endif
