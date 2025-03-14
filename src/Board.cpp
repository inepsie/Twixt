#include "Board.h"


Board::Board(size_t size) : m_size(size) {
    m_board.resize(size * size);
}

void board::id_2dto1d(size_t j, size_t i){

}

void board::id_1dto2d(size_t ind){
    return std::array<size_t, 2> ind_2D = {ind / m_size, ind % m_size};
}

void Board::play(size_t j, size_t i){
    m_board
}
