#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <tinyla/mat.hpp>
#include <tinyla/vec.hpp>
#include <array>
#include <cstddef>
#include <type_traits>

template<std::size_t N, typename T>
void compare(const tinyla::vec<N,T>& v1, const tinyla::vec<N,T>& v2)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(v1[i] == v2[i]);
    }
}

template<std::size_t N, typename T>
requires(std::is_floating_point_v<T>)
void compare(const tinyla::vec<N,T>& v1, const tinyla::vec<N,T>& v2)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(v1[i] == Catch::Approx(v2[i]));
    }
}

template<std::size_t N, typename T>
requires(std::is_floating_point_v<T>)
void compare(const tinyla::vec<N,T>& tgl_vec, const std::array<T,N>& glm_vec)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(tgl_vec[i] == Catch::Approx(glm_vec[i]));
    }
}

template<std::size_t N, typename T>
void compare(const tinyla::mat<N,T>& m, const std::array<std::array<T,N>,N>& a)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(m[i, j] == Catch::Approx(a[i][j]));
        }
    }
}

template<std::size_t N, typename T>
void compare(const tinyla::mat<N,T>& m1, const tinyla::mat<N,T>& m2)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(m1[i, j] == Catch::Approx(m2[i, j]));
        }
    }
}

#endif // COMPARE_HPP
