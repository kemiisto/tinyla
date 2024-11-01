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
    enum class vec_init {
        uninitialized,
        zero
    };

    template<std::size_t N, typename T>
    requires(N >= 2)
    class vec {
    public:
        constexpr explicit vec(vec_init init)
        {
            switch (init) {
                case vec_init::uninitialized:
                    break;
                case vec_init::zero:
                    set_to_zero();
                    break;
            }
        }

        constexpr vec(std::initializer_list<T> values)
        {
            assert(values.size() == N);
            auto it = values.begin();
            for (std::size_t i = 0; i < N; ++i) v[i] = *it++;
        }

        template<std::size_t M>
        constexpr vec(const vec<M, T>& smaller_vec, std::initializer_list<T> values) requires(M < N)
        {
            assert(values.size() == N - M);
            for (std::size_t i = 0; i < M; ++i) v[i] = smaller_vec[i];
            auto it = values.begin();
            for (std::size_t i = M; i < N; ++i) v[i] = *it++;
        }

        template<std::size_t M, typename...Ts>
        constexpr vec(const vec<M, T>& smaller_vec, Ts... vs) requires(M < N && sizeof...(Ts) == N - M)
            : vec(smaller_vec, std::initializer_list<T>{ vs... })
        {
        }

        void set_to_zero();

        constexpr void fill(const T& value) { v.fill(value); }

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

        constexpr vec& operator+=(vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::plus<T>());
            return *this;
        }

        constexpr friend vec operator+(vec vec1, vec vec2) noexcept
        {
            vec1 += vec2;
            return vec1;
        }

        constexpr vec& operator-=(vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::minus<T>());
            return *this;
        }

        constexpr friend vec operator-(vec vec1, vec vec2) noexcept
        {
            vec1 -= vec2;
            return vec1;
        }

        constexpr vec& operator*=(float a) noexcept
        {
            std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return a * c; });
            return *this;
        }

        constexpr friend vec operator*(float a, vec vec) noexcept
        {
            vec *= a;
            return vec;
        }

        constexpr friend vec operator*(vec vec, float a) noexcept
        {
            vec *= a;
            return vec;
        }

        constexpr vec& operator*=(vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::multiplies<T>());
            return *this;
        }

        constexpr friend vec operator*(vec vec1, vec vec2) noexcept
        {
            vec1 *= vec2;
            return vec1;
        }

        constexpr vec& operator/=(float a) noexcept
        {
            std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return c / a; });
            return *this;
        }

        constexpr friend vec operator/(vec vec, float a) noexcept
        {
            vec /= a;
            return vec;
        }

        constexpr vec& operator/=(vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::divides<T>());
            return *this;
        }

        constexpr friend vec operator/(vec vec1, vec vec2) noexcept
        {
            vec1 /= vec2;
            return vec1;
        }

        // Unary minus (negation)
        constexpr friend vec operator-(vec vec) noexcept
        {
            auto result = tinyla::vec<N,T>{tinyla::vec_init::uninitialized};
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), std::negate<T>());
            return result;
        }

        template<typename U>
        constexpr vec<N,U> cast() const noexcept
        {
            auto result = vec<N,U>{vec_init::uninitialized};
            for (auto i = std::size_t{0}; i < N; ++i) result[i] = static_cast<U>(v[i]);
            return result;
        }

        static constexpr T dot(vec vec1, vec vec2) noexcept
        {
            return std::inner_product(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), T{0});
        }

        static constexpr vec cross(vec vec1, vec vec2) noexcept requires(N == 3)
        {
            return {
                vec1.y() * vec2.z() - vec1.z() * vec2.y(),
                vec1.z() * vec2.x() - vec1.x() * vec2.z(),
                vec1.x() * vec2.y() - vec1.y() * vec2.x(),
            };
        }

        /**
        * Returns the unit normal vector of a plane spanned by vectors b - a and c - a.
        * Can be used to calculate a normal to a triangle with vertices a, b, c at point a.
        */
        static constexpr vec normal(vec a, vec b, vec c) noexcept requires(N == 3)
        {
            auto const ab = b - a;
            auto const ac = c - a;
            return cross(ab, ac).normalized();
        }

        /**
        * Returns the unit normal vector of a plane spanned by vectors vs[1] - vs[0] and vs[2] - vs[0].
        * Can be used to calculate a normal to a triangle with vertices vs[0], vs[1], vs[2] at point vs[0].
        */
        static constexpr vec normal(std::array<vec, 3> const& vs) noexcept requires(N == 3)
        {
            auto const& a = vs[0];
            auto const& b = vs[1];
            auto const& c = vs[2];
            return normal(a, b, c);
        }

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
}

template<std::size_t N, typename T>
requires(N >= 2)
void tinyla::vec<N, T>::set_to_zero()
{
    fill(T{0});
}

// If the vector is already normalized or zero, then this function simply returns it back.
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

#endif // TINYLA_VEC_H
