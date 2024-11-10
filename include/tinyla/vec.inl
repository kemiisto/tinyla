#ifndef TINYLA_VEC_INL
#define TINYLA_VEC_INL

#include <tinyla/util.hpp>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <numeric>

namespace tinyla
{
    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr vec<N, T>::vec(vec_init init)
    {
        switch (init) {
            case vec_init::uninitialized:
                break;
            case vec_init::zero:
                set_to_zero();
                break;
        }
    }

    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr vec<N, T>::vec(std::initializer_list<T> values)
    {
        assert(values.size() == N);
        auto it = values.begin();
        for (std::size_t i = 0; i < N; ++i) v[i] = *it++;
    }

    template<std::size_t N, typename T>
    requires(N >= 2)
    template<std::size_t M>
    constexpr vec<N, T>::vec(const vec<M, T>& smaller_vec, std::initializer_list<T> values) requires(M < N)
    {
        assert(values.size() == N - M);
        for (std::size_t i = 0; i < M; ++i) v[i] = smaller_vec[i];
        auto it = values.begin();
        for (std::size_t i = M; i < N; ++i) v[i] = *it++;
    }

    template<std::size_t N, typename T>
    requires(N >= 2)
    template<std::size_t M, typename...Ts>
    constexpr vec<N, T>::vec(const vec<M, T>& smaller_vec, Ts... vs) requires(M < N && sizeof...(Ts) == N - M)
        : vec(smaller_vec, std::initializer_list<T>{ vs... })
    {}

    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr void tinyla::vec<N, T>::set_to_zero()
    {
        fill(T{0});
    }

    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr void vec<N, T>::fill(const T& value) { v.fill(value); }

    template<std::size_t N, typename T>
    requires (N >= 2)
    tinyla::vec<N, T> tinyla::vec<N, T>::normalized() const noexcept
    {
        const float len = length();
        if (close_to_zero(len - 1.0f) || close_to_zero(len)) return *this;
        return *this / len;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    void tinyla::vec<N, T>::normalize() noexcept
    {
        const float len = length();
        if (!close_to_zero(len - 1.0f) && !close_to_zero(len)) *this /= len;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator+=(vec<N, T> rhs) noexcept
    {
        std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::plus<T>());
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator+(vec<N, T> vec1, vec<N, T> vec2) noexcept
    {
        vec1 += vec2;
        return vec1;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator-=(vec<N, T> rhs) noexcept
    {
        std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::minus<T>());
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator-(vec<N, T> vec1, vec<N, T> vec2) noexcept
    {
        vec1 -= vec2;
        return vec1;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator*=(float a) noexcept
    {
        std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return a * c; });
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator*(float a, vec<N, T> vec) noexcept
    {
        vec *= a;
        return vec;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator*(vec<N, T> vec, float a) noexcept
    {
        vec *= a;
        return vec;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator*=(vec<N, T> rhs) noexcept
    {
        std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::multiplies<T>());
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator*(vec<N, T> vec1, vec<N, T> vec2) noexcept
    {
        vec1 *= vec2;
        return vec1;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator/=(float a) noexcept
    {
        std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return c / a; });
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator/(vec<N, T> vec, float a) noexcept
    {
        vec /= a;
        return vec;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T>& vec<N, T>::operator/=(vec<N, T> rhs) noexcept
    {
        std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::divides<T>());
        return *this;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> operator/(vec<N, T> vec1, vec<N, T> vec2) noexcept
    {
        vec1 /= vec2;
        return vec1;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> vec<N, T>::operator-() noexcept
    {
        auto result = tinyla::vec<N,T>{vec_init::uninitialized};
        std::transform(v.begin(), v.end(), result.v.begin(), std::negate<T>());
        return result;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    template<typename U>
    constexpr vec<N, U> vec<N, T>::cast() const noexcept
    {
        auto result = vec<N, U>{vec_init::uninitialized};
        for (auto i = std::size_t{0}; i < N; ++i) result[i] = static_cast<U>(v[i]);
        return result;
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr T vec<N, T>::dot(vec<N, T> vec1, vec<N, T> vec2) noexcept
    {
        return std::inner_product(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), T{0});
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> vec<N, T>::cross(vec<N, T> vec1, vec<N, T> vec2) noexcept requires(N == 3)
    {
        return {
            vec1.y() * vec2.z() - vec1.z() * vec2.y(),
            vec1.z() * vec2.x() - vec1.x() * vec2.z(),
            vec1.x() * vec2.y() - vec1.y() * vec2.x(),
        };
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> vec<N, T>::normal(vec<N, T> a, vec<N, T> b, vec<N, T> c) noexcept requires(N == 3)
    {
        auto const ab = b - a;
        auto const ac = c - a;
        return cross(ab, ac).normalized();
    }

    template<std::size_t N, typename T>
    requires (N >= 2)
    constexpr vec<N, T> vec<N, T>::normal(std::array<vec<N, T>, 3> const& vs) noexcept requires(N == 3)
    {
        auto const& a = vs[0];
        auto const& b = vs[1];
        auto const& c = vs[2];
        return normal(a, b, c);
    }
}

#endif // TINYLA_VEC_INL
