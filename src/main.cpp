#include "Camera.h"
#include "Board.h"
#include "Constants.h"
#include "Quad.h"
#include "Noise.h"
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

static int _clickstate = 0;
static double _xy_mouse_double[2] = {90.0f, 90.0f};
static GLfloat _xy_mouse_float[2] = {90.0f, 90.0f};
static int _xy_mouse_int[2] = {0, 0};
static double _xy_quad[2] = {0, 0};

namespace C = Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

GLuint player = 0;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
  int x, y;
  int bsize = C::BOARD_SIZE;
  double f_bsize = (double) bsize;

    xpos = xpos - 1.0 * f_bsize;
    ypos = ypos - 1.0 * f_bsize;
    std::cout << "Position de la souris : (" << xpos << ", " << ypos << ")" << std::endl;
    x = (xpos) / C::QUAD_SIZE;
    y = (ypos) / C::QUAD_SIZE;

    _xy_quad[0] = (double)(C::BOARD_SIZE - x) * C::IQUAD_SIZE;
    _xy_quad[1] = (double)(C::BOARD_SIZE - y) * C::IQUAD_SIZE;

    //std::cout << "Position de la souris : (" << xpos << ", " << ypos << ")" << std::endl;
    std::cout << "Indices : (" << x << ", " << y << ")" << std::endl;
    std::cout << "_xy_quad : (" << _xy_quad[0] << ", " << _xy_quad[1] << ")" << std::endl;

}

void mouse_button_callback(GLFWwindow *window, int button, int action,
                           int mods) {

  Board &board = Board::getInstance();
  size_t x, y;
  float fboard_size = (double) C::BOARD_SIZE;
  float fw_height = (double) C::WINDOW_HEIGHT;
  float fw_width = (double) C::WINDOW_WIDTH;
  glfwGetCursorPos(window, &_xy_mouse_double[0], &_xy_mouse_double[1]);
  // if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS){
  if (button == GLFW_MOUSE_BUTTON_LEFT) {
    _clickstate = (_clickstate + 1) % 2;
    glfwGetCursorPos(window, &_xy_mouse_double[0], &_xy_mouse_double[1]);

    //std::cout << "1Mouse : " << _xy_mouse_double[0] << ",   " << _xy_mouse_double[1] << std::endl;

    _xy_mouse_double[0] = fabs(_xy_mouse_double[0] - fw_width + 1.5 * fboard_size);
    _xy_mouse_double[1] = fabs(_xy_mouse_double[1] - fw_height + 1.5 * fboard_size);

    x = (size_t) (((_xy_mouse_double[0] + fboard_size) / (fw_width + 1.0))  * fboard_size);
    y = (size_t) (((_xy_mouse_double[1] + fboard_size)  / (fw_height + 1.0)) * fboard_size);
    //std::cout << "3Mouse : " << _xy_mouse_double[0] << ",   " << _xy_mouse_double[1] << std::endl;
    //std::cout << "2x = " << x << ",      y = " << y << std::endl;
  }

  if(_clickstate == 1){
    //board.reset(player);
    board.play(player, y, x);
    player = (player + 1) % 4;
  }
}

static inline void init() {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  Board &board = Board::getInstance();
  board.init(C::BOARD_SIZE);
  // Compute Shader - Map Compute
  //shader_manager.loadShader("mapCompute", "../res/shaders/map.comp");
  // Map Draw Shader
  shader_manager.loadShader("boardDraw", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

static inline void draw_cube_repere(Camera cam) {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  std::shared_ptr<Shader> shader = shader_manager.getShader("cubeRepere");
  Quad c = Quad(shader);
  c.setPosition(glm::vec3(_xy_quad[0], _xy_quad[1], 0));
  c.draw(cam.get_proj(), cam.get_view());
  glBindVertexArray(0);
  shader->stop();
}

static inline void draw(Camera cam){
  Board &board = Board::getInstance();
  glClearColor(0.7f, 0.7f, 0.75f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  board.draw(cam);
  draw_cube_repere(cam);
}


static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 300.0;
  GLfloat vit = 0.2;
  glm::vec2 transl = glm::vec2(1.0, 0.0);
  cam.update(
      glm::vec3(1.2 * dist * sin(vit * current_time), dist * 0.5, dist * cos(vit * current_time)),
      glm::vec3(0.2 * dist, 0.1 * dist, 0.2 * dist), glm::vec3(0.0, 1.0, 0.0));
  cam.update(glm::vec3(transl.x, transl.y, -10.0), glm::vec3(transl.x, transl.y, 0), glm::vec3(0, 1, 0));
}

int main() {
  // GLFW
  if (glfwInit() != GLFW_TRUE) {
    return -1;
  }
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_SAMPLES, 4);
  glEnable(GL_MULTISAMPLE);
  GLFWwindow *window =
      glfwCreateWindow(C::WINDOW_WIDTH, C::WINDOW_HEIGHT,
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
  Camera cam = Camera(glm::vec3(0, 0, -40), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  auto lastTime = std::chrono::high_resolution_clock::now();
  // Fonction callback pour gérer les mouse clicks
  glfwSetMouseButtonCallback(window, mouse_button_callback);
  // Fontion callback pcofour avoir les positions mouse
  glfwSetCursorPosCallback(window, cursor_position_callback);

  while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currentTime - lastTime).count();
    camera_settings(cam, dt);
    draw(cam);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
