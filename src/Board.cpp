#include <array>
#include <cstddef>
#include <cstdlib>
#include <memory>
#include <random>
#include "Board.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Shader.h"

// Définition du `unique_ptr`
std::unique_ptr<Board> Board::board = Board::createBoard();

Board::Board() {
    m_size = 0;  // Initialisation par défaut
}

// Définition du destructeur
Board::~Board() {
    m_board.clear();
}

// Définition des autres méthodes...
Board& Board::getInstance() {
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

void Board::init(size_t size) {
    // Initialisation du générateur de nombres aléatoires
    //std::random_device rd;
    m_size = size;
    m_size_2 = size * size;
    m_board.resize(m_size_2);
    m_links.resize(m_size_2);
    for(int i=0 ; i < m_size_2 ; ++i){
    //std::mt19937 gen(rd());  // Générateur basé sur random_device
    //std::uniform_int_distribution<GLuint> dis(1, 3); // Distribution entre 0 et 2
    m_board[i] = 1;
    }

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, m_size_2 * sizeof(GLuint), m_board.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
}


std::array<size_t, 2> Board::id_1dto2d(size_t ind) {
    return {ind % m_size, ind / m_size};
}

size_t Board::id_2dto1d(size_t j, size_t i) {
    return j * m_size + i;
}


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
    for(int i=0 ; i<m_board.size() ; ++i){
        m_board[i] = valeur;
    }

    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    GLuint* ptr = (GLuint*) glMapBufferRange(GL_ARRAY_BUFFER, offset, size,
                                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

    if (ptr) {
        std::fill(ptr, ptr + size, valeur); // Remplit tout le buffer avec la nouvelle couleur
        glUnmapBuffer(GL_ARRAY_BUFFER);
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Board::play(size_t j, size_t i) {
    size_t ind = id_2dto1d(j, i);
    size_t offset = ind * sizeof(GLuint);
    size_t size = m_size_2 * sizeof(GLuint);
    GLuint * ptr = nullptr;
    GLuint val = 2 + (m_turn % 2);
    if(unbound(j, i)) return;
    if(m_board[ind] != 1) return;

    std::cout << "TURN : " << 2 + (m_turn % 2) << std::endl;
    m_board[ind] = val;
    glBindBuffer(GL_ARRAY_BUFFER, m_buffer);

    ptr = (GLuint*) glMapBufferRange(GL_ARRAY_BUFFER, offset, sizeof(GLuint),
                                 GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_RANGE_BIT);

if (ptr) {
        memcpy(ptr, &val, sizeof(GLuint)); // Écriture en mémoire GPU
        glUnmapBuffer(GL_ARRAY_BUFFER); // Fin du mapping
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    ++m_turn;
}

bool Board::unbound(size_t ind){
  if(ind < 0) return true;
  if(ind >= m_size_2) return true;
  return false;
}

bool Board::unbound(size_t j, size_t i){
  if(i < 0) return true;
  if(j < 0) return true;
  if(i >= m_size) return true;
  if(j >= m_size) return true;
  return false;
}

std::array<size_t, 2> Board::link_ind(size_t type, size_t j, size_t i){
  std::array<size_t, 2> coords;
  switch (type) {
    case 0 :return (coords = {j-2, i+1});
    case 1 :return (coords = {j-1, i+2});
    case 2 :return (coords = {j+1, i+2});
    case 3 :return (coords = {j+2, i+1});
    case 4 :return (coords = {j+2, i-1});
    case 5 :return (coords = {j+1, i-2});
    case 6 :return (coords = {j-1, i-2});
    case 7 :return (coords = {j-2, i-1});
    default:
      throw std::runtime_error("Board::link_ind, switch case default, cas non prévu");
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
