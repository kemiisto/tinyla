#ifndef TINYLA_VEC_H
#define TINYLA_VEC_H

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
    class mat;

    enum class vec_init {
        uninitialized,
        zero
    };

    template<std::size_t N, typename T>
    requires(N >= 2)
    class vec {
    public:
        constexpr explicit vec(vec_init init);
        constexpr vec(std::initializer_list<T> values);

        template<std::size_t M>
        constexpr vec(const vec<M, T>& smaller_vec, std::initializer_list<T> values) requires(M < N);

        template<std::size_t M, typename...Ts>
        constexpr vec(const vec<M, T>& smaller_vec, Ts... vs) requires(M < N && sizeof...(Ts) == N - M);

        constexpr void set_to_zero();
        constexpr void fill(const T& value);

        constexpr T& operator[](std::size_t i) { return v[i]; }
        constexpr T operator[](std::size_t i) const { return v[i]; }

        // x, y, z, w components
        constexpr T& x() { return v[0]; }
        constexpr T x() const { return v[0]; }

        constexpr T& y() { return v[1]; }
        constexpr T y() const { return v[1]; }

        constexpr T& z() requires(N >= 3) { return v[2]; }
        constexpr T z() const requires(N >= 3) { return v[2]; }

        constexpr T& w() requires(N >= 4) { return v[3]; }
        constexpr T w() const requires(N >= 4) { return v[3]; }

        // r, g, b, a components
        constexpr T& r() { return v[0]; }
        constexpr T r() const { return v[0]; }

        constexpr T& g() { return v[1]; }
        constexpr T g() const { return v[1]; }

        constexpr T& b() requires(N >= 3) { return v[2]; }
        constexpr T b() const requires(N >= 3) { return v[2]; }

        constexpr T& a() requires(N >= 4) { return v[3]; }
        constexpr T a() const requires(N >= 4) { return v[3]; }

        constexpr T* data() noexcept { return v.data(); }
        constexpr const T* data() const noexcept { return v.data(); }

        constexpr T length() const noexcept { return std::sqrt(dot(*this, *this)); }
        constexpr T length_squared() const noexcept { return dot(*this, *this); }

        vec normalized() const noexcept;
        void normalize() noexcept;

        // Element-wise binary operations

        constexpr vec& operator+=(vec rhs) noexcept;
        constexpr friend vec operator+ <>(vec vec1, vec vec2) noexcept;

        constexpr vec& operator-=(vec rhs) noexcept;
        constexpr friend vec operator- <>(vec vec1, vec vec2) noexcept;

        constexpr vec& operator*=(float a) noexcept;
        constexpr friend vec operator* <>(float a, vec vec) noexcept;
        constexpr friend vec operator* <>(vec vec, float a) noexcept;

        constexpr vec& operator*=(vec rhs) noexcept;
        constexpr friend vec operator* <>(vec vec1, vec vec2) noexcept;

        constexpr vec& operator/=(float a) noexcept;
        constexpr friend vec operator/ <>(vec vec, float a) noexcept;

        constexpr vec& operator/=(vec rhs) noexcept;
        constexpr friend vec operator/ <>(vec vec1, vec vec2) noexcept;

        // Unary minus (negation)
        constexpr vec operator-() noexcept;

        template<typename U>
        constexpr vec<N,U> cast() const noexcept;

        /**
        * Returns the unit normal vector of a plane spanned by vectors b - a and c - a.
        * Can be used to calculate a normal to a triangle with vertices a, b, c at point a.
        */
        static constexpr vec normal(vec a, vec b, vec c) noexcept requires(N == 3);

        /**
        * Returns the unit normal vector of a plane spanned by vectors vs[1] - vs[0] and vs[2] - vs[0].
        * Can be used to calculate a normal to a triangle with vertices vs[0], vs[1], vs[2] at point vs[0].
        */
        static constexpr vec normal(std::array<vec, 3> const& vs) noexcept requires(N == 3);

        friend vec<N,T> operator* <>(const mat<N,T>& a, const vec<N,T>& b);
        friend constexpr T dot <>(vec<N, T> vec1, vec<N, T> vec2) noexcept;
    private:
        std::array<T, N> v;
    };

    // deduction guide similar to std::array
    // https://en.cppreference.com/w/cpp/container/array/deduction_guides
    template<typename T, typename... U>
    vec(T, U...) -> vec<1 + sizeof...(U), T>;

    using vec2i = vec<2, int>;
    using vec3i = vec<3, int>;
    using vec4i = vec<4, int>;

    using vec2f = vec<2, float>;
    using vec3f = vec<3, float>;
    using vec4f = vec<4, float>;

    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr T dot(vec<N, T> vec1, vec<N, T> vec2) noexcept;

    template<std::size_t N, typename T>
    requires(N >= 2)
    constexpr vec<N, T> cross(vec<N, T> vec1, vec<N, T> vec2) noexcept requires(N == 3);
}

#include "vec.inl"

#endif // TINYLA_VEC_H
