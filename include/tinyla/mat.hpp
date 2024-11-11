#ifndef TINYLA_MAT_H
#define TINYLA_MAT_H

#include <tinyla/vec.hpp>
#include <initializer_list>
#include <iostream>

namespace tinyla
{
    enum class mat_init {
        uninitialized,
        zero,
        identity,
        diagonal
    };

    template<std::size_t N, typename T>
    requires(N >= 2)
    class mat;

    template<std::size_t N, typename T>
    requires(N >= 2)
    mat<N, T> operator*(const mat<N,T>& a, const mat<N,T>& b);

    template<std::size_t N, typename T>
    requires(N >= 2)
    vec<N,T> operator*(const mat<N,T>& m, const vec<N,T>& v);

    template<std::size_t N, typename T>
    requires(N >= 2)
    class mat {
    public:
        constexpr explicit mat(mat_init init, vec<N,T> v = vec<N,T>{vec_init::zero});
        constexpr mat(std::initializer_list<T> values);

        constexpr T& operator()(std::size_t row, std::size_t column);
        constexpr T operator()(std::size_t row, std::size_t column) const;

        constexpr mat operator*=(const mat& other);

        constexpr void set_to_zero();
        constexpr void set_to_identity();
        constexpr void set_to_diagonal(vec<N,T> const& v);

        constexpr T determinant() const requires (N == 4);
        constexpr mat inverted() const requires (N == 4);

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        constexpr bool close_to(const mat& other);

        friend mat<N,T> operator* <>(const mat<N,T>& a, const mat<N,T>& b);
        friend vec<N,T> operator* <>(const mat<N,T>& a, const vec<N,T>& b);
    private:
        T m[N][N];
    };

    using mat2i = mat<2,int>;
    using mat3i = mat<3,int>;
    using mat4i = mat<4,int>;

    using mat2f = mat<2,float>;
    using mat3f = mat<3,float>;
    using mat4f = mat<4,float>;
}

#include "mat.inl"

#endif // TINYLA_MAT_H
