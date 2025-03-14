#include <array>
#include <cstdlib>
#include <memory>
#include <random>
#include "Board.h"
#include "Camera.h"
#include "ShaderManager.h"
#include "Shader.h"

// Définition du `unique_ptr`
std::unique_ptr<Board> Board::board = Board::createBoard();

// Définition du constructeur
Board::Board() {
    m_size = 0;  // Initialisation par défaut
}

// Définition du destructeur
Board::~Board() {
    // Si nécessaire, libérer des ressources ici
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
    std::random_device rd;
    m_size = size;
    m_board.resize(size * size);
    for(int i=0 ; i<m_board.size() ; ++i){
    std::mt19937 gen(rd());  // Générateur basé sur random_device
    std::uniform_int_distribution<GLuint> dis(0, 1); // Distribution entre 0 et 1
    m_board[i] = dis(gen);
    std::cout << m_board[i] << std::endl;
    }

  glGenVertexArrays(1, &m_vao);
  glBindVertexArray(m_vao);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, m_board.size(), m_board.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);
}

std::array<size_t, 2> Board::id_1dto2d(size_t ind) {
    return {ind / m_size, ind % m_size};
}

size_t Board::id_2dto1d(size_t j, size_t i) {
    return j * m_size + i;
}

void Board::play(size_t j, size_t i){
}

void Board::draw(Camera cam) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("boardDraw");
  shader->use();

  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(m_vao); // Associer VAO
  shader->set_uniform("MVP", mvp);
  shader->set_uniform("view", cam.get_view());

  glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, m_buffer);
  glDrawArrays(GL_POINTS, 0, m_board.size()); // 1 vertex par bloc

  shader->stop();
  glBindVertexArray(0);
  glUseProgram(0);
}
