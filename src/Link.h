#ifndef LINK_H_
#define LINK_H_

class Link {
public:
    Link(size_t size);
    ~Link();

private:
    size_t m_i, m_j;
    std::vector<GLubyte> m_board;
};


#endif // LINK_H_
