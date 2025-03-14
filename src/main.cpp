#include "Camera.h"
#include "Board.h"
#include "Constants.h"
#include "Cube.h"
#include "Noise.h"
#include "Shader.h"
#include "ShaderManager.h"
#include "Texture.h"
#include "glm/gtx/transform.hpp"
#include <GL/glew.h> // first
#include <GLFW/glfw3.h>
#include <chrono>
#include <functional>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <math.h>
#include <vector>

namespace C = Constants; //  Pour ne pas à avoir à écrire Constants:: à chaque fois

static inline void init() {
  ShaderManager &shader_manager = ShaderManager::getInstance();
  Board &board = Board::getInstance();
  board.init(24);
  // Compute Shader - Map Compute
  shader_manager.loadShader("mapCompute", "../res/shaders/map.comp");
  // Map Draw Shader
  shader_manager.loadShader("mapDraw", "../res/shaders/voxels.vert",
                            "../res/shaders/voxels.frag",
                            "../res/shaders/voxels.geom");
  // Cube Repère Shader
  shader_manager.loadShader("cubeRepere", "../res/shaders/cube_repere.vert",
                            "../res/shaders/cube_repere.frag");
  glEnable(GL_DEPTH_TEST);
}

static inline void draw(Camera cam){
  glClearColor(0.0f, 0.1f, 0.2f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

static inline void camera_settings(Camera &cam, float current_time) {
  static GLfloat angle = 6.0;
  GLfloat dist = 300.0;
  GLfloat vit = 0.2;
  cam.update(
      glm::vec3(1.2 * dist * sin(vit * current_time), dist * 0.5, dist * cos(vit * current_time)),
      glm::vec3(0.2 * dist, 0.1 * dist, 0.2 * dist), glm::vec3(0.0, 1.0, 0.0));
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
  Camera cam = Camera(glm::vec3(0, 0, -10), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
  auto lastTime = std::chrono::high_resolution_clock::now();
  while (glfwGetKey(window, GLFW_KEY_L) != GLFW_PRESS &&
         glfwWindowShouldClose(window) == 0) {
    auto currentTime = std::chrono::high_resolution_clock::now();
    float dt = std::chrono::duration<float>(currentTime - lastTime).count();
    //camera_settings(cam, dt);
    draw(cam);
    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}
