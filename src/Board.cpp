#include "Board.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderManager.h"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <random>

// Définition du `unique_ptr`
std::unique_ptr<Board> Board::board = Board::createBoard();

Board::Board() {
}

// Définition du destructeur
Board::~Board() { m_board.clear(); }

Board &Board::getInstance() {
  if (!board) {
    board = std::make_unique<Board>();
  }
  return *board;
}

void Board::reset() {
  board.reset(); // Détruit l'instance
}

std::unique_ptr<Board> Board::createBoard() {
  return std::make_unique<Board>();
}

void Board::print_board(){
    int k = 0;
    for(int i=0 ; i<m_size ; ++i){
        for(int j=0 ; j<m_size ; ++j){
            std::cout << m_board[k] << "  ";
            ++k;
        }
        std::cout << std::endl;
    }
}

void Board::init(size_t size) {
  m_size = size;
  m_size_2 = size * size;
  m_board.resize(m_size_2, 1);
  m_links.resize(m_size_2);

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, m_size_2 * sizeof(GLuint), m_board.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
}

std::array<size_t, 2> Board::id_1dto2d(size_t ind) {
  return {ind % m_size, ind / m_size};
}

size_t Board::id_2dto1d(size_t i, size_t j) { return j * m_size + i; }

void Board::draw(Camera cam) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("boardDraw");
  shader->use();

  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(m_vao); // Associer VAO
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());
  shader->set_uniform("board_size", (int)m_size);

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_buffer);
  glDrawArrays(GL_POINTS, 0, m_size_2); // 1 vertex par bloc

  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}

void Board::reset(GLuint valeur) {
  size_t offset = 0;
  size_t size = m_size_2 * sizeof(GLuint);
  for (int i = 0; i < m_board.size(); ++i) {
    m_board[i] = valeur;
  }
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

  GLuint *ptr = (GLuint *)glMapBufferRange(GL_ARRAY_BUFFER, offset, size,
                                           GL_MAP_WRITE_BIT |
                                               GL_MAP_INVALIDATE_RANGE_BIT);
  if (ptr) {
    std::fill(ptr, ptr + size,
              valeur); // Remplit tout le buffer avec la nouvelle couleur
    glUnmapBuffer(GL_ARRAY_BUFFER);
  }
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Board::play(size_t i, size_t j) {
  size_t ind = id_2dto1d(i, j);
  size_t offset = ind * sizeof(GLuint);
  size_t size = m_size_2 * sizeof(GLuint);
  GLuint *ptr = nullptr;
  GLuint val = 2 + (m_turn % 2);
  if (unbound(i, j))
    return;
  if (m_board[ind] != 1)
    return;
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
  m_turn += 1;
  m_player = (m_player + 1) % 2;
}

size_t Board::reverse_ind_link(size_t type){
    return (type + 4) % 8;
}

void Board::add_link(size_t type, size_t i, size_t j, size_t ni, size_t nj){
    size_t ind = id_2dto1d(i, j);
    size_t reverse = reverse_ind_link(type);
    m_links[ind].m_link[type] = m_turn;
    ind = id_2dto1d(ni, nj);
    m_links[ind].m_link[reverse] = m_turn;
    std::cout << "addlink : " << i << ", " << j << "  --->     " << ni << ", " << nj << std::endl;
}

void Board::check_links(size_t i, size_t j){
    std::array<size_t, 2> coords;
    size_t coords_1D;
    for(size_t n=0 ; n<8 ; ++n){ //  On test les 8 directions
        coords = link_ind(n, i, j); // coordonnées à tester
        coords_1D = id_2dto1d(i, j); // conversion 1D
        std::cout << "board1 : " << m_board[coords_1D] << ",   turn : " << (m_player + 2) << std::endl;
        std::cout << "board 2 : " << m_board[coords_1D] << ",   turn : " << (m_player + 2) << std::endl;
        std:: cout << "IJ :    " << i << ",  " << j << std::endl;
        std:: cout << "COORDS :    " << coords[0] << ",  " << coords[1] << std::endl;
        if(m_board[coords_1D] != (m_player + 3)) continue; // test de pions pour link
        if(unbound(coords[0], coords[1])) continue; // test si coordonnées valides
        add_link(n, i, j, coords[0], coords[1]);
    }
}


bool Board::unbound(size_t ind) {
  if (ind < 0)
    return true;
  if (ind >= m_size_2)
    return true;
  return false;
}

bool Board::unbound(size_t x, size_t y) {
  if (x < 0)
    return true;
  if (y < 0)
    return true;
  if (x >= m_size)
    return true;
  if (y >= m_size)
    return true;
  return false;
}

std::array<size_t, 2> Board::link_ind(size_t type, size_t x, size_t y) {
  std::array<size_t, 2> coords;
  switch (type) {
  case 0:
    return (coords = {x + 1, y - 2});
  case 1:
    return (coords = {x + 2, y - 1});
  case 2:
    return (coords = {x + 2, y + 1});
  case 3:
    return (coords = {x + 1, y + 2});
  case 4:
    return (coords = {x - 1, y + 2});
  case 5:
    return (coords = {x - 2, y + 1});
  case 6:
    return (coords = {x - 2, y - 1});
  case 7:
    return (coords = {x - 1, y - 2});
  default:
    throw std::runtime_error(
        "Board::link_ind, switch case default, cas non prévu");
    return (coords = {0, 0});
  }
}
/*      (7)         (0)
 *          *     *
 *           *   *
 * (6) *     *   *     * (1)
 *       * *   *   * *
 *           * O *
 *       * *   *   * *
 * (5) *     *   *     * (2)
 *           *   *
 *          *     *
 *       (4)        (3)
 *
 */
