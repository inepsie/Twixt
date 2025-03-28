#include "Board.h"
#include "Constants.h"
#include "Mouse.h"
#include "Camera.h"
#include "Shader.h"
#include "ShaderManager.h"
#include <array>
#include <cstddef>
#include <cstdlib>
#include <glm/fwd.hpp>
#include <memory>
#include <random>
#include <algorithm>

// Définition du `unique_ptr`
std::unique_ptr<Board> Board::board = Board::createBoard();

Board::Board() {
}

// Définition du destructeur
Board::~Board() {}

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
        std::cout << "----------------------------------------------------" << std::endl;
}

void Board::print_links(){
    int k = 0;
    for(int i=0 ; i<m_size ; ++i){
        for(int j=0 ; j<m_size ; ++j){
            for(int n=0 ; n<8 ; ++n){
                std::cout << m_links[k][n] << ", ";
            }
                std::cout << "|  ";
            ++k;
        }
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
        std::cout << std::endl;
    }
        std::cout << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout << "----------------------------------------------------" << std::endl;
        std::cout <<  std::endl;
        std::cout <<  std::endl;
}

void Board::init(size_t size) {
    std::fill(m_board.begin(), m_board.end(), 1); // Remplit `m_board` avec 1
    m_board[0] = 0;// Coins
    m_board[C::BOARD_SIZE - 1] = 0;
    m_board[C::BOARD_SIZE_2 - C::BOARD_SIZE] = 0;
    m_board[C::BOARD_SIZE_2 - 1] = 0;

  // Points
  glGenVertexArrays(1, &m_pointVAO);
  glBindVertexArray(m_pointVAO);
  glEnableVertexAttribArray(0);
  glGenBuffers(1, &m_buffer);
  glBindBuffer(GL_ARRAY_BUFFER, m_buffer);
  glBufferData(GL_ARRAY_BUFFER, m_size_2 * sizeof(GLuint), m_board.data(),
               GL_STATIC_DRAW);
  glVertexAttribPointer(0, 1, GL_UNSIGNED_INT, GL_FALSE, sizeof(GLuint),
                        (const void *)0);

  init_lines();
}


void Board::init_lines() {
    glGenVertexArrays(1, &m_lineVAO);
    glGenBuffers(1, &m_lineVBO);
    glGenBuffers(1, &m_colorVBO); // Générer un buffer pour les couleurs

    glBindVertexArray(m_lineVAO);

    // VBO pour les positions
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(glm::vec3), m_lines.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    // VBO pour les couleurs
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_linesColors.size() * sizeof(glm::vec3), m_linesColors.data(), GL_DYNAMIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    add_edge_lines();
}

void Board::add_edge_lines(){
  //TODO FACTORISER
    Mouse &mouse = Mouse::getInstance();
    std::array<double, 2> pos;
    glm::vec3 color;
    size_t i1, j1, i2, j2;
    // Ajouter les points
    i1=0, j1=0, i2=0, j2=C::BOARD_SIZE-1;
    color = glm::vec3(0.2, 0.2, 0.2);
    pos = mouse.ind_int_to_vec3(i1, j1);
    pos[1] += C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0] + C::NORM_QUAD_SIZE/2, pos[1], 0.0));
    m_linesColors.push_back(color); // Associer une couleur

    pos = mouse.ind_int_to_vec3(i2, j2);
    pos[1] -= C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0] + C::NORM_QUAD_SIZE/2, pos[1], 0.0));
    m_linesColors.push_back(color); // Même couleur pour le deuxième point
    i1=C::BOARD_SIZE-1, i2=C::BOARD_SIZE-1;

    pos = mouse.ind_int_to_vec3(i1, j1);
    pos[1] += C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0] - C::NORM_QUAD_SIZE/2, pos[1], 0.0));
    m_linesColors.push_back(color); // Associer une couleur

    pos = mouse.ind_int_to_vec3(i2, j2);
    pos[1] -= C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0] - C::NORM_QUAD_SIZE/2, pos[1], 0.0));
    m_linesColors.push_back(color); // Même couleur pour le deuxième point

    // Ajouter les points
    i1=0, j1=0, i2=C::BOARD_SIZE-1, j2=0;
    color = glm::vec3(0.75, 0.0, 0.0);
    pos = mouse.ind_int_to_vec3(i1, j1);
    pos[0] += C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0], pos[1] + C::NORM_QUAD_SIZE/2, 0.0));
    m_linesColors.push_back(color); // Associer une couleur

    pos = mouse.ind_int_to_vec3(i2, j2);
    pos[0] -= C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0], pos[1] + C::NORM_QUAD_SIZE/2, 0.0));
    m_linesColors.push_back(color); // Même couleur pour le deuxième point
    j1=C::BOARD_SIZE-1, j2=C::BOARD_SIZE-1;

    pos = mouse.ind_int_to_vec3(i1, j1);
    pos[0] += C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0], pos[1] - C::NORM_QUAD_SIZE/2, 0.0));
    m_linesColors.push_back(color); // Associer une couleur

    pos = mouse.ind_int_to_vec3(i2, j2);
    pos[0] -= C::NORM_QUAD_SIZE * 0.75;
    m_lines.push_back(glm::vec3(pos[0], pos[1] - C::NORM_QUAD_SIZE/2, 0.0));
    m_linesColors.push_back(color); // Même couleur pour le deuxième point

    // Mettre à jour le VBO des positions
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(glm::vec3), m_lines.data(), GL_DYNAMIC_DRAW);

    // Mettre à jour le VBO des couleurs
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_linesColors.size() * sizeof(glm::vec3), m_linesColors.data(), GL_DYNAMIC_DRAW);
}

void Board::add_line(size_t i, size_t j, size_t ni, size_t nj) {
    Mouse &mouse = Mouse::getInstance();
    std::array<double, 2> pos;
    glm::vec3 color;

    if(m_player == 1) color = glm::vec3(0.2, 0.2, 0.2);
    else{color = glm::vec3(0.75, 0.0, 0.0);}
    // Ajouter les points
    pos = mouse.ind_int_to_vec3(i, j);
    m_lines.push_back(glm::vec3(pos[0], pos[1], 0.0));
    m_linesColors.push_back(color); // Associer une couleur

    pos = mouse.ind_int_to_vec3(ni, nj);
    m_lines.push_back(glm::vec3(pos[0], pos[1], 0.0));
    m_linesColors.push_back(color); // Même couleur pour le deuxième point

    // Mettre à jour le VBO des positions
    glBindBuffer(GL_ARRAY_BUFFER, m_lineVBO);
    glBufferData(GL_ARRAY_BUFFER, m_lines.size() * sizeof(glm::vec3), m_lines.data(), GL_DYNAMIC_DRAW);

    // Mettre à jour le VBO des couleurs
    glBindBuffer(GL_ARRAY_BUFFER, m_colorVBO);
    glBufferData(GL_ARRAY_BUFFER, m_linesColors.size() * sizeof(glm::vec3), m_linesColors.data(), GL_DYNAMIC_DRAW);
}

void Board::draw_lines(Camera cam) {
    ShaderManager &shader_manager = ShaderManager::getInstance();
    std::shared_ptr<Shader> shader = shader_manager.getShader("linesDraw");
    shader->use();

    glm::mat4 mvp = cam.get_proj() * cam.get_view();
    glBindVertexArray(m_lineVAO); // Associer VAO
    shader->set_uniform("MVP", mvp);

    glLineWidth(5.0f);

    glDrawArrays(GL_LINES, 0, m_lines.size());

    glLineWidth(1.0f);
    shader->stop();
    glBindVertexArray(0);
    glUseProgram(0);
}

std::array<size_t, 2> Board::id_1dto2d(size_t ind) {
  return {ind % m_size, ind / m_size};
}

size_t Board::id_2dto1d(size_t i, size_t j) { return j * m_size + i; }

void Board::draw(Camera cam) {
    draw_lines(cam);
    draw_points(cam);
}

void Board::draw_points(Camera cam) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("pointsDraw");
  shader->use();

  glm::mat4 mvp = cam.get_proj() * cam.get_view();
  glBindVertexArray(m_pointVAO); // Associer VAO
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

int Board::win() {
  size_t type = 0;
  std::array<size_t, 2> ind2D;
  size_t ind;
  std::vector<std::array<size_t, 2>> visited;
  std::vector<std::array<size_t, 2>> to_visit;
  if (m_player == 0 && (m_top.size() < 1 || m_bot.size() < 1))
    return 0;
  if (m_player == 1 && (m_left.size() < 1 || m_right.size() < 1))
    return 0;
  std::cout << "TEST WIN" << std::endl;
  // propagation à partir des bords + sauvegarde des indices pour ne pas boucler
  ind2D[0] = 0, ind2D[1] = 0;
  for (size_t i = 0; i < m_top.size(); ++i) {
    to_visit.push_back({m_top[i], 0});
  }
  //while(to_visite.size()>0){
  for (size_t i = 0; i < m_top.size(); ++i) {
    if(ind2D[0] = C::BOARD_SIZE -1) return 1;
    ind2D[0] = m_top[i];
    to_visit.push_back(ind2D);
    /*
    if (std::find(visited.begin(), visited.end(), ind) != visited.end()) {
      continue;
    }
    */
    for(size_t n=0 ; n<8 ; ++n){
    std::cout << "BOUCLE" << std::endl;
      ind = id_2dto1d(ind2D[0], ind2D[1]);// Conversion 2D to 1D
      type = m_links[ind][n];// On va chercher le type de lien
      to_visit.push_back(link_ind(type, ind2D[0], ind2D[1])); // On push la nouvelle coordonnée trouver
    }
    for(size_t n=0 ; n<to_visit.size() ; ++n){
      std::cout << "to visit : " << to_visit[n][0] << ", " << to_visit[n][1] << std::endl;
    }
    visited.push_back(ind2D);
  }
}

void Board::add_win_pawn(size_t i, size_t j){
  if((m_player)==0 && (j!=0 && j!=C::BOARD_SIZE-1))return;
  if((m_player)==1 && (i!=0 && i!=C::BOARD_SIZE-1))return;
  if((m_player)==0 && j==0) m_top.push_back(i);
  if((m_player)==0 && j!=0) m_bot.push_back(i);
  if((m_player)==1 && i==0) m_left.push_back(j);
  if((m_player)==1 && i!=0) m_right.push_back(j);
}

void Board::print_win_pawn(){
  std::cout << "------------------------------" << std::endl;
  std::cout << "WIN PAWN : " << std::endl;
  for(size_t i=0 ; i<m_top.size() ; ++i){
    std::cout << "RED - top " << m_top[i] << std::endl;
  }
  for(size_t i=0 ; i<m_bot.size() ; ++i){
    std::cout << "RED - bot " << m_bot[i] << std::endl;
  }
  for(size_t i=0 ; i<m_left.size() ; ++i){
    std::cout << "BLACK - left " << m_left[i] << std::endl;
  }
  for(size_t i=0 ; i<m_right.size() ; ++i){
    std::cout << "BLACK - right " << m_right[i] << std::endl;
  }
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
  add_win_pawn(i, j);
  print_win_pawn();
  std::cout << "WIN ?  -> " << win() << std::endl;

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

std::array<int, 2> Board::rot90ind(size_t nb_rot, int i, int j){
    nb_rot = nb_rot % 4;
    for(size_t n=0 ; n<nb_rot ; ++n){
        int save = i;
        i = -j;
        j = save;
    }
    return {i, j};
}

void Board::block_link(size_t type, size_t i, size_t j) {
    std::array<int, 2> ncoords = {};
    size_t nb_rot = type / 2;
    size_t k = 9 * (type % 2);// Pour le 2eme type d'offsets

    struct Offset {
        int di, dj, link;
    };
    static const std::vector<Offset> offsets = {
        {0, -1, 1}, {0, -1, 2}, {0, -1, 3}, // Premier groupe (i, j-1)
        {1, -1, 5}, {1, -1, 6}, {1, -1, 7}, // Deuxième groupe (i+1, j-1)
        {1, 0, 6}, {1, 0, 7},               // Troisième groupe (i+1, j)
        {2, -1, 6},                            // Dernier groupe (i+2, j+1)
        //
        {1, 0, 5}, {1, 0, 6}, {1, 0, 7}, // Premier groupe (i+1, j)
        {1, -1, 3}, {1, -1, 2}, {1, -1, 1}, // Deuxième groupe (i+1, j-1)
        {0, -1, 2}, {0, -1, 1},               // Troisième groupe (i, j+1)
        {2, 0, 6}                            // Dernier groupe (i+2, j)
    };

    auto block_single_link = [&](size_t i, size_t j, size_t link) {
        size_t ind = id_2dto1d(i, j);
        if (!unbound(ind)) {
            m_links[ind][link] = 9;
            size_t nind = link_ind_1D(link, ind);
            if (!unbound(nind)) {
                m_links[nind][reverse_link(link)] = 9;
            }
        }
    };

    for (size_t n=0; n<9; ++n) {
        ncoords = rot90ind(nb_rot, offsets[n+k].di, offsets[n+k].dj);
        block_single_link(i + ncoords[0], j + ncoords[1],
                          (type + offsets[n+k].link) % 8);
    }
}

size_t Board::reverse_link(size_t type){
    return (type + 4) % 8;
}

void Board::add_link(size_t type, size_t i, size_t j, size_t ni, size_t nj){
    size_t ind = id_2dto1d(i, j);
    size_t reverse = reverse_link(type);
    add_line(C::BOARD_SIZE - 1 - i, C::BOARD_SIZE - 1 - j, C::BOARD_SIZE - 1 - ni, C::BOARD_SIZE - 1 - nj);
    m_links[ind][type] = m_player + 2;
    ind = id_2dto1d(ni, nj);
    m_links[ind][reverse] = m_player + 2;
    block_link(type, i, j);
}

void Board::check_links(size_t i, size_t j){
    std::array<size_t, 2> coords_other;
    size_t coords_1D;
    size_t coords;
    for(size_t n=0 ; n<8 ; ++n){ //  On test les 8 directions
        coords = id_2dto1d(i, j);
        coords_other = link_ind(n, i, j); // coordonnées à tester
        coords_1D = id_2dto1d(coords_other[0], coords_other[1]); // conversion 1D
        if(m_board[coords_1D] != (m_player + 2)) continue; // test de pions pour link
        if(m_links[coords][n] == 9) continue; // lien bloqué par un autre lien
        if(unbound(coords_other[0], coords_other[1])) continue; // test si coordonnées valides
        add_link(n, i, j, coords_other[0], coords_other[1]);
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

size_t Board::link_ind_1D(size_t type, size_t ind) {
    std::array<size_t, 2> xy = id_1dto2d(ind);
    std::array<size_t, 2> nxy = link_ind(type, xy[0], xy[1]);
    return id_2dto1d(nxy[0], nxy[1]);
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
