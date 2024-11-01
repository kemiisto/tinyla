#ifndef COMPARE_HPP
#define COMPARE_HPP

#include <tinyla/mat.hpp>
#include <tinyla/vec.hpp>
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
void compare(const tinyla::vec<N,T>& tglVec, const glm::vec<N,T>& glmVec)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(tglVec[i] == Catch::Approx(glmVec[i]));
    }
}

#endif // COMPARE_HPP
