#ifndef LINK_H_
#define LINK_H_

#include "GL/glew.h"
#include <cstddef> // Pour size_t
#include <vector>  // Pour std::vector

class Link {
public:
  Link();
  ~Link();
  GLuint m_link[8] = {};

private:
};

#endif // LINK_H_
