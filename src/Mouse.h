#ifndef MOUSE_H_
#define MOUSE_H_

#include <array>
#include <iostream>
#include <string>

#define GLEW_STATIC
#include "GL/glew.h"
#include "glm/glm.hpp"

class Mouse {
public:
  static Mouse &
  getInstance() { //  Pour avoir une instance unique et statique à la class
    static Mouse instance;
    return instance;
  }
  void set_clickstate(int val);
  void set_xy_mouse_double(double x, double y);
  void set_xy_ind(size_t x, size_t y);
  void set_xy_quad(double x, double y);
  void update(double x, double y);
  int get_clickstate();
  std::array<double, 2> get_xy_mouse_double();
  std::array<size_t, 2> get_xy_ind();
  std::array<double, 2> get_xy_quad();

private:
  // Empêche la copie
  Mouse() = default;
  ~Mouse() = default; // Plus besoin de destructeur manuel

  int m_clickstate = 0;
  double m_xy_mouse_double[2] = {0.0f, 0.0f};
  size_t m_xy_ind[2] = {0, 0};
  double m_xy_quad[2] = {0, 0};
};

#endif // MOUSE_H_
