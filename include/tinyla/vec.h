#ifndef TINYLA_VEC_H
#define TINYLA_VEC_H

#include <tinyla/util.h>
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <functional>
#include <initializer_list>
#include <numeric>

namespace tinyla
{
    enum class VecInit {
        Uninitialized,
        Zero
    };

    template<typename T, typename... Types>
    concept is_all_same = (... && std::is_same<T, Types>::value);

    template<std::size_t N, typename T = float>
    requires(N >= 2)
    class Vec
    {
    public:
        constexpr explicit Vec(VecInit init) {
            switch (init) {
                case VecInit::Uninitialized:
                    break;
                case VecInit::Zero:
                    setToZero();
                    break;
            }
        }
        constexpr Vec(std::initializer_list<T> values) {
            assert(values.size() == N);
            auto it = values.begin();
            for (std::size_t i = 0; i < N; ++i) v[i] = *it++;
        }

        void setToZero();

        template<is_all_same<T>... Types>
        requires (sizeof...(Types) == N)
        constexpr Vec(Types... args) : v {args...} {}

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
        constexpr T lengthSquared() const noexcept { return dot(*this, *this); }

        Vec normalized() const noexcept;
        void normalize() noexcept;

        // Element-wise binary operations

        constexpr inline Vec& operator+=(Vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::plus<T>());
            return *this;
        }

        constexpr friend inline Vec operator+(Vec vec1, Vec vec2) noexcept
        {
            vec1 += vec2;
            return vec1;
        }

        constexpr inline Vec& operator-=(Vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::minus<T>());
            return *this;
        }

        constexpr friend inline Vec operator-(Vec vec1, Vec vec2) noexcept
        {
            vec1 -= vec2;
            return vec1;
        }

        constexpr inline Vec& operator*=(float a) noexcept
        {
            std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return a * c; });
            return *this;
        }

        constexpr friend inline Vec operator*(float a, Vec vec) noexcept
        {
            vec *= a;
            return vec;
        }

        constexpr friend inline Vec operator*(Vec vec, float a) noexcept
        {
            vec *= a;
            return vec;
        }

        constexpr inline Vec& operator*=(Vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::multiplies<T>());
            return *this;
        }

        constexpr friend inline Vec operator*(Vec vec1, Vec vec2) noexcept
        {
            vec1 *= vec2;
            return vec1;
        }

        constexpr inline Vec& operator/=(float a) noexcept
        {
            std::transform(v.begin(), v.end(), v.begin(), [a](auto& c) { return c / a; });
            return *this;
        }

        constexpr friend inline Vec operator/(Vec vec, float a) noexcept
        {
            vec /= a;
            return vec;
        }

        constexpr inline Vec& operator/=(Vec rhs) noexcept
        {
            std::transform(v.begin(), v.end(), rhs.v.begin(), v.begin(), std::divides<T>());
            return *this;
        }

        constexpr friend inline Vec operator/(Vec vec1, Vec vec2) noexcept
        {
            vec1 /= vec2;
            return vec1;
        }

        // Unary minus (negation)
        constexpr friend inline Vec operator-(Vec vec) noexcept
        {
            Vec result{tinyla::VecInit::Uninitialized};
            std::transform(vec.v.begin(), vec.v.end(), result.v.begin(), std::negate<T>());
            return result;
        }

        static constexpr T dot(Vec vec1, Vec vec2) noexcept
        {
            return std::inner_product(vec1.v.begin(), vec1.v.end(), vec2.v.begin(), T{0});
        }

    private:
        std::array<T, N> v;
    };

    // deduction guide
    template<typename T, typename... U>
    Vec(T, U...) -> Vec<1 + sizeof...(U), T>;

    using Vec2i = Vec<2, int>;
    using Vec3i = Vec<3, int>;
    using Vec4i = Vec<4, int>;

    using Vec2f = Vec<2, float>;
    using Vec3f = Vec<3, float>;
    using Vec4f = Vec<4, float>;
}

template<std::size_t N, typename T = float>
requires(N >= 2)
void tinyla::Vec<N, T>::setToZero() {
    fill(T{0});
}

// If the vector is already normalized or zero, then this function simply returns it back.
template<std::size_t N, typename T>
requires (N >= 2)
tinyla::Vec<N, T> tinyla::Vec<N, T>::normalized() const noexcept
{
    const float len = length();
    if (closeToZero(len - 1.0f) || closeToZero(len)) {
        return *this;
    } else {
        return *this / len;
    }
}

template<std::size_t N, typename T>
requires (N >= 2)
void tinyla::Vec<N, T>::normalize() noexcept
{
    const float len = length();
    if (!closeToZero(len - 1.0f) && !closeToZero(len)) {
        *this /= len;
    }
}

#endif // TINYLA_VEC_H
