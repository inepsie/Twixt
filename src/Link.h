#ifndef LINK_H_
#define LINK_H_

#include "GL/glew.h"
#include <cstddef>  // Pour size_t
#include <vector>   // Pour std::vector

class Link {
public:
    Link(size_t size);
    ~Link();

private:
    size_t m_i, m_j;
    std::vector<GLubyte> m_board;
};


#endif // LINK_H_
