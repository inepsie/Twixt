#pragma once
#include <array>
#include <cstddef>

namespace utils {

constexpr std::array<size_t, 2> id_1dto2d(size_t ind, size_t size) {
    return { ind % size, ind / size };
}

constexpr size_t id_2dto1d(size_t i, size_t j, size_t size) {
    return j * size + i;
}
} // namespace utils
