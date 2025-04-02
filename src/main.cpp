#include "Board.h"
#include "Camera.h"
#include "Constants.h"
#include "Link.h"
#include "Mouse.h"
#include "Noise.h"
#include "Quad.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "glm/gtx/transform.hpp"
#include <GL/glew.h> // first
#include <GLFW/glfw3.h>
#include <chrono>
#include <cmath>
#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include <array>   // Pour std::array
#include <cstddef> // Pour size_t
#include <random>
#include <thread>

namespace C = Constants;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
  Mouse &mouse = Mouse::getInstance();
  mouse.update(xpos, ypos);
}

void mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
  Board &board = Board::getInstance();
  Mouse &mouse = Mouse::getInstance();
  std::array<size_t, 2> coords;
  if (button == GLFW_MOUSE_BUTTON_LEFT) mouse.change_clickstate();

  if (mouse.get_clickstate()) {
    coords = mouse.get_xy_ind();
    board.play(coords[0], coords[1]);
    //board.print_board();
    //board.print_links();
  }
}

static inline void init() {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  Board &board = Board::getInstance();
  Mouse &mouse = Mouse::getInstance();
  board.init();
  // Points Draw Shader
  shader_manager.loadShader("pointsDraw", "../res/shaders/points.vert",
                            "../res/shaders/points.frag",
                            "../res/shaders/points.geom");
  // Lines Draw Shader
  shader_manager.loadShader("linesDraw", "../res/shaders/lines.vert",
                            "../res/shaders/lines.frag");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static inline void draw_quad(Camera cam) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  Mouse &mouse = Mouse::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("cubeRepere");
  Quad c = Quad(shader);
  std::array<double, 2> xy_quad;
  xy_quad = mouse.get_xy_quad();
  c.setPosition(glm::vec3(xy_quad[0], xy_quad[1], 0));
  c.draw(cam.get_proj(), cam.get_view());
  glBindVertexArray(0);
  shader->stop();
}

static inline void draw(Camera cam) {
  Board &board = Board::getInstance();
  glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  board.draw(cam);
  draw_quad(cam);
}

static inline void camera_settings(Camera &cam) {
  glm::vec2 transl = glm::vec2(1.0, 0.0);
  cam.update(glm::vec3(transl.x, transl.y, -10.0),
             glm::vec3(transl.x, transl.y, 0), glm::vec3(0, 1, 0));
}

size_t random_size_t(size_t a, size_t N) {
    static std::random_device rd;  // Génère une vraie entropie
    static std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_int_distribution<size_t> dist(a, N); // Distribution uniforme entre 0 et N
    return dist(gen);
}

static inline void auto_random_play(){
    Board &board = Board::getInstance();
    size_t imin, imax, jmin, jmax;
    imin = 1; imax = C::BOARD_SIZE-2; jmin = 0; jmax = C::BOARD_SIZE-1;
    if(board.get_player()==1){
        imin = jmin;
        imax = jmax;
        jmin = 1;
        jmax = C::BOARD_SIZE-2;
    }
    size_t i = random_size_t(imin, imax);
    size_t j = random_size_t(jmin, jmax);

    if(!board.ended) board.play(i, j);
    else std::cout << "Victoire joueur " << board.ended - 1 << std::endl;
}

int main() {
  Camera cam = Camera(glm::vec3(0, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  // GLFW
  if (glfwInit() != GLFW_TRUE) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 32);
  glEnable(GL_MULTISAMPLE);
  GLFWwindow *window = glfwCreateWindow(C::WINDOW_WIDTH, C::WINDOW_HEIGHT,
                                        "Twixt - Explo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    return -100;
  }
  glfwMakeContextCurrent(window);

  // GLEW
  if (glewInit() != GLEW_OK) {
    glfwTerminate();
    return -200;
  }

  // OpenGL API
  glViewport(0, 0, C::WINDOW_WIDTH, C::WINDOW_HEIGHT);
  init();
  auto lastTime = std::chrono::high_resolution_clock::now();
  // Fonction callback pour gérer les mouse clicks
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  // Fontion callback pcofour avoir les positions mouse
  glfwSetCursorPosCallback(window, cursor_position_callback);
  camera_settings(cam);

  while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currentTime - lastTime).count();
    auto_random_play();
    draw(cam);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
