#include "Mouse.h"
#include "Constants.h"
#include <array>

namespace C = Constants;

void Mouse::change_clickstate(){
    m_clickstate = (m_clickstate + 1) % 2;
}


void Mouse::set_clickstate(int val){
    m_clickstate = val;
}

void Mouse::set_xy_mouse_double(double x, double y){
    m_xy_mouse_double[0] = x;
    m_xy_mouse_double[1] = y;
}

void Mouse::set_xy_ind(size_t x, size_t y){
    m_xy_ind[0] = x;
    m_xy_ind[1] = y;
}

void Mouse::set_xy_quad(double x, double y){
    m_xy_quad[0] = x;
    m_xy_quad[1] = y;
}

int Mouse::get_clickstate(){
    return m_clickstate;
}

std::array<double, 2> Mouse::get_xy_mouse_double(){
    std::array<double, 2> xy = {m_xy_mouse_double[0], m_xy_mouse_double[1]};
    return xy;
}

std::array<size_t, 2> Mouse::get_xy_ind(){
    std::array<size_t, 2> xy = {m_xy_ind[0], m_xy_ind[1]};
    return xy;
}

std::array<double, 2> Mouse::get_xy_quad(){
    std::array<double, 2> xy = {m_xy_quad[0], m_xy_quad[1]};
    return xy;
}

std::array<double, 2> Mouse::ind_int_to_vec3(size_t i, size_t j){
    std::array<double, 2> pos;
    pos[0] = (double)(i + 1) * C::NORM_QUAD_SIZE;
    pos[1] = (double)(j + 1) * C::NORM_QUAD_SIZE;
    return pos;
}

void Mouse::update(double x, double y){
    m_xy_mouse_double[0] = x - 0.25 * C::QUAD_SIZE;
    m_xy_mouse_double[1] = y - 0.25 * C::QUAD_SIZE;

    m_xy_ind[0] = (m_xy_mouse_double[0]) / C::QUAD_SIZE;
    m_xy_ind[1] = (m_xy_mouse_double[1]) / C::QUAD_SIZE;

    m_xy_quad[0] = (double)(C::BOARD_SIZE - m_xy_ind[0]) * C::NORM_QUAD_SIZE;
    m_xy_quad[1] = (double)(C::BOARD_SIZE - m_xy_ind[1]) * C::NORM_QUAD_SIZE;
}
