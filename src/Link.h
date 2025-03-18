#ifndef LINK_H_
#define LINK_H_

#include "GL/glew.h"
#include <cstddef> // Pour size_t
#include <vector>  // Pour std::vector

class Link {
public:
  Link();
  ~Link();

private:
  GLuint m_link[8] = {};
};

#endif // LINK_H_
