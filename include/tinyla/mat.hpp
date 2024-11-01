#ifndef TINYLA_MATRIX_H
#define TINYLA_MATRIX_H

#include <tinyla/vec.hpp>
#include <tinyla/util.hpp>
#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace tinyla
{
    enum class mat_init {
        uninitialized,
        identity,
        diagonal
    };

    template<std::size_t N, typename T>
    requires(N >= 3)
    class mat;

    template<std::size_t N, typename T>
    requires(N >= 3)
    mat<N, T> operator*(const mat<N,T>& a, const mat<N,T>& b);

    template<std::size_t N, typename T>
    requires(N >= 3)
    vec<N,T> operator*(const vec<N,T>& v, const mat<N,T>& m);

    template<std::size_t N, typename T>
    requires(N >= 3)
    vec<N,T> operator*(const mat<N,T>& m, const vec<N,T>& v);

    template<std::size_t N, typename T>
    requires(N >= 3)
    class mat
    {
    public:
        static mat scaling(const vec<N-1,T>& s) requires (N == 4);
        static mat translation(const vec<N-1,T>& t) requires (N == 4);
        static mat rotation(T angle, const vec<N-1,T>& axis) requires (N == 4);
        static mat perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane) requires(N == 4);

        explicit mat(mat_init init, vec<N,T> v = vec<N,T>{vec_init::zero});
        mat(std::initializer_list<T> values);

        constexpr T& operator()(std::size_t i, std::size_t j);
        constexpr T operator()(std::size_t i, std::size_t j) const;

        mat operator*=(const mat& o);

        void set_to_identity();
        void set_to_diagonal(vec<N,T> const& v);

        void pre_scale(T s) requires (N == 4);
        void post_scale(T s) requires (N == 4);

        void pre_scale(const vec<N - 1,T>& s) requires (N == 4);
        void post_scale(const vec<N - 1,T>& s) requires (N == 4);

        void pre_translate(const vec<N-1,T>& t) requires (N == 4);
        void post_translate(const vec<N-1,T>& t) requires (N == 4);

        void pre_rotate(T angle, const vec<N-1,T>& axis) requires (N == 4);
        void post_rotate(T angle, const vec<N-1,T>& axis) requires (N == 4);

        T determinant() const requires (N == 4);
        mat inverted() const requires (N == 4);

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        bool close_to(const mat& other);

        friend mat<N,T> operator* <>(const mat<N,T>& a, const mat<N,T>& b);
    private:
        T m[N][N]{};

        void pre_rotate_x(T c, T s) requires (N == 4);
        void post_rotate_x(T c, T s) requires (N == 4);

        void pre_rotate_y(T c, T s) requires (N == 4);
        void post_rotate_y(T c, T s) requires (N == 4);

        void pre_rotate_z(T c, T s) requires (N == 4);
        void post_rotate_z(T c, T s) requires (N == 4);
    };

    using mat3i = mat<3,int>;
    using mat4i = mat<4,int>;

    using mat3f = mat<3,float>;
    using mat4f = mat<4,float>;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N,T>::mat(mat_init init, vec<N,T> v)
{
    switch (init) {
        case mat_init::uninitialized:
            break;
        case mat_init::identity:
            set_to_identity();
            break;
        case mat_init::diagonal:
            set_to_diagonal(v);
            break;
    }
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N,T>::mat(std::initializer_list<T> values)
{
    assert(values.size() == N*N);
    auto it = values.begin();
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            m[j][i] = *it++;
        }
    }
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr T& tinyla::mat<N,T>::operator()(std::size_t i, std::size_t j)
{
    return m[i][j];
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr T tinyla::mat<N,T>::operator()(std::size_t i, std::size_t j) const
{
    return m[i][j];
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::set_to_identity()
{
    auto i = tinyla::vec<N,T>{vec_init::uninitialized};
    i.fill(T{1});
    set_to_diagonal(i);
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::set_to_diagonal(vec<N,T> const& v)
{
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (i == j) {
                m[i][j] = v[i];
            } else {
                m[i][j] = T{0};
            }
        }
    }
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_translate(const vec<N - 1,T>& t) requires (N == 4)
{
    /**
     * | 1  0  0  tx |   | m00   m01   m02   m03 |   | m00 + m30*tx   m01 + m31*tx   m02 + m32*tx   m03 + m33*tx |
     * | 0  1  0  ty |   | m10   m11   m12   m13 |   | m10 + m30*ty   m11 + m31*ty   m12 + m32*ty   m13 + m33*ty |
     * | 0  0  1  tz | * | m20   m21   m22   m23 | = | m20 + m30*ty   m21 + m31*ty   m22 + m32*ty   m23 + m33*ty |
     * | 0  0  0  1  |   | m30   m31   m32   m33 |   | m30            m31            m32            m33          |
     *
     * All columns are changing.
     */
    m[0][0] += m[0][3] * t.x();
    m[0][1] += m[0][3] * t.y();
    m[0][2] += m[0][3] * t.z();

    m[1][0] += m[1][3] * t.x();
    m[1][1] += m[1][3] * t.y();
    m[1][2] += m[1][3] * t.z();

    m[2][0] += m[2][3] * t.x();
    m[2][1] += m[2][3] * t.y();
    m[2][2] += m[2][3] * t.z();

    m[3][0] += m[3][3] * t.x();
    m[3][1] += m[3][3] * t.y();
    m[3][2] += m[3][3] * t.z();
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_translate(const vec<N - 1,T>& t) requires (N == 4)
{
    /**
     * | m00   m01   m02   m03 |   | 1  0  0  tx |   | m00   m01   m02   m00*tx + m01*ty + m02*tz + m03 |
     * | m10   m11   m12   m13 |   | 0  1  0  ty |   | m10   m11   m12   m10*tx + m11*ty + m12*tz + m13 |
     * | m20   m21   m22   m23 | * | 0  0  1  tz | = | m20   m21   m22   m20*tx + m21*ty + m22*tz + m23 |
     * | m30   m31   m32   m33 |   | 0  0  0  1  |   | m30   m31   m32   m30*tx + m31*ty + m32*tz + m33 |
     *
     * Only the last column is changing.
     */
    m[3][0] += m[0][0] * t.x() + m[1][0] * t.y() + m[2][0] * t.z();
    m[3][1] += m[0][1] * t.x() + m[1][1] * t.y() + m[2][1] * t.z();
    m[3][2] += m[0][2] * t.x() + m[1][2] * t.y() + m[2][2] * t.z();
    m[3][3] += m[0][3] * t.x() + m[1][3] * t.y() + m[2][3] * t.z();
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_scale(T s) requires (N == 4)
{
    preScale({s, s, s});
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_scale(T s) requires (N == 4)
{
    postScale({s, s, s});
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_scale(const vec<N - 1,T>& s) requires (N == 4)
{
    /**
     * | sx  0   0   0 |   | m00   m01   m02   m03 |   | m00*sx   m01*sx   m02*sx   m03*sx |
     * | 0   sy  0   0 |   | m10   m11   m12   m13 |   | m10*sy   m11*sy   m12*sy   m13*sy |
     * | 0   0   sz  0 | * | m20   m21   m22   m23 | = | m20*sz   m21*sz   m22*sz   m23*sz |
     * | 0   0   0   1 |   | m30   m31   m32   m33 |   | m30      m31      m32      m33    |
     *
     * All the columns except the last one are changing.
     * Remember the column-major order!
     */
    m[0][0] *= s.x();
    m[0][1] *= s.y();
    m[0][2] *= s.z();

    m[1][0] *= s.x();
    m[1][1] *= s.y();
    m[1][2] *= s.z();

    m[2][0] *= s.x();
    m[2][1] *= s.y();
    m[2][2] *= s.z();

    m[3][0] *= s.x();
    m[3][1] *= s.y();
    m[3][2] *= s.z();
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_scale(const vec<N - 1,T>& s) requires (N == 4)
{
    /**
     * | m00   m01   m02   m03 |   | sx  0   0   0 |   | m00*sx   m01*sy   m02*sz   m03 |
     * | m10   m11   m12   m13 |   | 0   sy  0   0 |   | m10*sx   m11*sy   m12*sz   m13 |
     * | m20   m21   m22   m23 | * | 0   0   sz  0 | = | m20*sx   m21*sy   m22*sz   m23 |
     * | m30   m31   m32   m33 |   | 0   0   0   1 |   | m30*sx   m31*sy   m32*sz   m33 |
     *
     * All the columns except the last one are changing.
     * Remember the column-major order!
     */
    m[0][0] *= s.x();
    m[0][1] *= s.x();
    m[0][2] *= s.x();
    m[0][3] *= s.x();

    m[1][0] *= s.y();
    m[1][1] *= s.y();
    m[1][2] *= s.y();
    m[1][3] *= s.y();

    m[2][0] *= s.z();
    m[2][1] *= s.z();
    m[2][2] *= s.z();
    m[2][3] *= s.z();
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_rotate_x(T c, T s) requires (N == 4)
{
    /**
     * | 1   0   0   0 |   | m00   m01   m02   m03 |   | m00           m01           m02           m03         |
     * | 0   c  -s   0 |   | m10   m11   m12   m13 |   | m10*c-m20*s   m11*c-m21*s   m12*c-m22*s   m13*c-m23*s |
     * | 0   s   c   0 | * | m20   m21   m22   m23 | = | m20*c+m10*s   m21*c+m11*s   m22*c+m12*s   m23*c+m13*s |
     * | 0   0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
     */
    T tmp;
    m[0][1] = (tmp = m[0][1]) * c - m[0][2] * s;    m[0][2] = m[0][2] * c + tmp * s;
    m[1][1] = (tmp = m[1][1]) * c - m[1][2] * s;    m[1][2] = m[1][2] * c + tmp * s;
    m[2][1] = (tmp = m[2][1]) * c - m[2][2] * s;    m[2][2] = m[2][2] * c + tmp * s;
    m[3][1] = (tmp = m[3][1]) * c - m[3][2] * s;    m[3][2] = m[3][2] * c + tmp * s;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_rotate_x(T c, T s) requires (N == 4)
{
    /**
     * | m00   m01   m02   m03 |   | 1   0   0   0 |   | m00    m01*c + m02*s    -m01*s + m02*c   m03 |
     * | m10   m11   m12   m13 |   | 0   c  -s   0 |   | m10    m11*c + m12*s    -m11*s + m12*c   m13 |
     * | m20   m21   m22   m23 | * | 0   s   c   0 | = | m20    m21*c + m22*s    -m21*s + m22*c   m23 |
     * | m30   m31   m32   m33 |   | 0   0   0   1 |   | m30    m31*c + m32*s    -m31*s + m32*c   m33 |
     */

    T tmp;
    m[1][0] = (tmp = m[1][0]) * c + m[2][0] * s;    m[2][0] = -tmp * s + m[2][0] * c;
    m[1][1] = (tmp = m[1][1]) * c + m[2][1] * s;    m[2][1] = -tmp * s + m[2][1] * c;
    m[1][2] = (tmp = m[1][2]) * c + m[2][2] * s;    m[2][2] = -tmp * s + m[2][2] * c;
    m[1][3] = (tmp = m[1][3]) * c + m[2][3] * s;    m[2][3] = -tmp * s + m[2][3] * c;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_rotate_y(T c, T s) requires (N == 4)
{
    /**
     * |  c   0   s   0 |   | m00   m01   m02   m03 |   | m00*c+m20*s   m01*c+m21*s   m02*c+m22*s   m03*c+m23*s |
     * |  0   1   0   0 |   | m10   m11   m12   m13 |   | m10           m11           m12           m13         |
     * | -s   0   c   0 | * | m20   m21   m22   m23 | = | m20*c-m00*s   m21*c-m01*s   m22*c-m02*s   m23*c-m03*s |
     * |  0   0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
     */
    T tmp;
    m[0][0] = (tmp = m[0][0]) * c + m[0][2] * s;    m[0][2] = m[0][2] * c - tmp * s;
    m[1][0] = (tmp = m[1][0]) * c + m[1][2] * s;    m[1][2] = m[1][2] * c - tmp * s;
    m[2][0] = (tmp = m[2][0]) * c + m[2][2] * s;    m[2][2] = m[2][2] * c - tmp * s;
    m[3][0] = (tmp = m[3][0]) * c + m[3][2] * s;    m[3][2] = m[3][2] * c - tmp * s;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_rotate_y(T c, T s) requires (N == 4)
{
    /**
     * | m00   m01   m02   m03 |   |  c   0   s   0 |   | m00*c - m02*s   m01   m00*s + m02*c   m03 |
     * | m10   m11   m12   m13 |   |  0   1   0   0 |   | m10*c - m12*s   m11   m10*s + m12*c   m13 |
     * | m20   m21   m22   m23 | * | -s   0   c   0 | = | m20*c - m22*s   m21   m20*s + m22*c   m23 |
     * | m30   m31   m32   m33 |   |  0   0   0   1 |   | m30*c - m32*s   m31   m30*s + m32*c   m33 |
     */
    T tmp;
    m[0][0] = (tmp = m[0][0]) * c - m[2][0] * s;    m[2][0] = tmp * s + m[2][0] * c;
    m[0][1] = (tmp = m[0][1]) * c - m[2][1] * s;    m[2][1] = tmp * s + m[2][1] * c;
    m[0][2] = (tmp = m[0][2]) * c - m[2][2] * s;    m[2][2] = tmp * s + m[2][2] * c;
    m[0][3] = (tmp = m[0][3]) * c - m[2][3] * s;    m[2][3] = tmp * s + m[2][3] * c;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_rotate_z(T c, T s) requires (N == 4)
{
    /**
     * | c   -s   0   0 |   | m00   m01   m02   m03 |   | m00*c-m10*s   m01*c-m11*s   m02*c-m12*s   m03*c-m13*s |
     * | s    c   0   0 |   | m10   m11   m12   m13 |   | m10*c+m00*s   m11*c+m01*s   m12*c+m02*s   m13*c+m03*s |
     * | 0    0   1   0 | * | m20   m21   m22   m23 | = | m20           m21           m22           m23         |
     * | 0    0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
     */
    T tmp;
    m[0][0] = (tmp = m[0][0]) * c - m[0][1] * s;  m[0][1] = m[0][1] * c + tmp * s;
    m[1][0] = (tmp = m[1][0]) * c - m[1][1] * s;  m[1][1] = m[1][1] * c + tmp * s;
    m[2][0] = (tmp = m[2][0]) * c - m[2][1] * s;  m[2][1] = m[2][1] * c + tmp * s;
    m[3][0] = (tmp = m[3][0]) * c - m[3][1] * s;  m[3][1] = m[3][1] * c + tmp * s;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_rotate_z(T c, T s) requires (N == 4)
{
    /**
     * | m00   m01   m02   m03 |   | c   -s   0   0 |   | m00*c + m01*s   -m00*s - m01*c   m02   m03 |
     * | m10   m11   m12   m13 |   | s    c   0   0 |   | m10*c + m11*s   -m10*s - m11*c   m12   m13 |
     * | m20   m21   m22   m23 | * | 0    0   1   0 | = | m20*c + m21*s   -m20*s - m21*c   m22   m23 |
     * | m30   m31   m32   m33 |   | 0    0   0   1 |   | m30*c + m31*s   -m30*s - m31*c   m32   m33 |
     */

    T tmp;
    m[0][0] = (tmp = m[0][0]) * c + m[1][0] * s;    m[1][0] = -tmp * s + m[1][0] * c;
    m[0][1] = (tmp = m[0][1]) * c + m[1][1] * s;    m[1][1] = -tmp * s + m[1][1] * c;
    m[0][2] = (tmp = m[0][2]) * c + m[1][2] * s;    m[1][2] = -tmp * s + m[1][2] * c;
    m[0][3] = (tmp = m[0][3]) * c + m[1][3] * s;    m[1][3] = -tmp * s + m[1][3] * c;
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::pre_rotate(T angle, const vec<N - 1,T>& axis)  requires (N == 4)
{
    const T a = deg_to_rad(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    if (x == T{0}) {
        if (y == T{0}) {
            if (z != T{0}) {
                pre_rotate_z(c, s);
                return;
            }
        } else if (z == T{0}) {
            pre_rotate_y(c, s);
            return;
        }
    } else if (y == T{0} && z == T{0}) {
        pre_rotate_x(c, s);
        return;
    }

    *this = mat::rotation(angle, axis) * (*this);
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::mat<N,T>::post_rotate(T angle, const vec<N - 1,T>& axis) requires (N == 4)
{
    const T a = deg_to_rad(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    if (x == T{0}) {
        if (y == T{0}) {
            if (z != T{0}) {
                post_rotate_z(c, s);
                return;
            }
        } else if (z == T{0}) {
            post_rotate_y(c, s);
            return;
        }
    } else if (y == T{0} && z == T{0}) {
        post_rotate_x(c, s);
        return;
    }

    *this *= mat::rotation(angle, axis);
}

// Calculate the determinant of a 2x2 sub-matrix.
template<typename T>
static inline T det2(const T m[4][4], int col0, int col1, int row0, int row1)
{
    return m[col0][row0] * m[col1][row1] - m[col0][row1] * m[col1][row0];
}

// Calculate the determinant of a 3x3 sub-matrix.
template<typename T>
static inline T det3(const T m[4][4],
                     int col0, int col1, int col2,
                     int row0, int row1, int row2)
{
    return m[col0][row0] * det2(m, col1, col2, row1, row2)
        - m[col1][row0] * det2(m, col0, col2, row1, row2)
        + m[col2][row0] * det2(m, col0, col1, row1, row2);
}

// Calculate the determinant of a 4x4 matrix.
template<typename T>
static inline T det4(const T m[4][4])
{
    T det;
    det  = m[0][0] * det3(m, 1, 2, 3, 1, 2, 3);
    det -= m[1][0] * det3(m, 0, 2, 3, 1, 2, 3);
    det += m[2][0] * det3(m, 0, 1, 3, 1, 2, 3);
    det -= m[3][0] * det3(m, 0, 1, 2, 1, 2, 3);
    return det;
}

template<std::size_t N, typename T>
requires(N >= 3)
T tinyla::mat<N,T>::determinant() const requires (N == 4)
{
    return det4(m);
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N, T> tinyla::mat<N,T>::inverted() const requires (N == 4)
{
    mat<N,T> inv(mat_init::uninitialized);

    T det = det4(m);
    if (close_to_zero(det)) {
        return tinyla::mat<N,T>(mat_init::identity);
    }

    det = T{1} / det;

    inv.m[0][0] = det3(m, 1, 2, 3, 1, 2, 3) * det;
    inv.m[0][1] = -det3(m, 0, 2, 3, 1, 2, 3) * det;
    inv.m[0][2] = det3(m, 0, 1, 3, 1, 2, 3) * det;
    inv.m[0][3] = -det3(m, 0, 1, 2, 1, 2, 3) * det;

    inv.m[1][0] = -det3(m, 1, 2, 3, 0, 2, 3) * det;
    inv.m[1][1] = det3(m, 0, 2, 3, 0, 2, 3) * det;
    inv.m[1][2] = -det3(m, 0, 1, 3, 0, 2, 3) * det;
    inv.m[1][3] = det3(m, 0, 1, 2, 0, 2, 3) * det;

    inv.m[2][0] = det3(m, 1, 2, 3, 0, 1, 3) * det;
    inv.m[2][1] = -det3(m, 0, 2, 3, 0, 1, 3) * det;
    inv.m[2][2] = det3(m, 0, 1, 3, 0, 1, 3) * det;
    inv.m[2][3] = -det3(m, 0, 1, 2, 0, 1, 3) * det;

    inv.m[3][0] = -det3(m, 1, 2, 3, 0, 1, 2) * det;
    inv.m[3][1] = det3(m, 0, 2, 3, 0, 1, 2) * det;
    inv.m[3][2] = -det3(m, 0, 1, 3, 0, 1, 2) * det;
    inv.m[3][3] = det3(m, 0, 1, 2, 0, 1, 2) * det;

    return inv;
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr T* tinyla::mat<N,T>::data() noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr const T* tinyla::mat<N,T>::data() const noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires (N >= 3)
tinyla::mat<N, T> tinyla::mat<N, T>::scaling(vec<N - 1, T> const& s) requires (N == 4)
{
    /**
     * | sx  0   0   0 |
     * | 0   sy  0   0 |
     * | 0   0   sz  0 |
     * | 0   0   0   1 |
     */
    mat<N,T> mat(mat_init::uninitialized);

    mat.m[0][0] = s.x();
    mat.m[0][1] = T{0};
    mat.m[0][2] = T{0};
    mat.m[0][3] = T{0};

    mat.m[1][0] = T{0};
    mat.m[1][1] = s.y();
    mat.m[1][2] = T{0};
    mat.m[1][3] = T{0};

    mat.m[2][0] = T{0};
    mat.m[2][1] = T{0};
    mat.m[2][2] = s.z();
    mat.m[2][3] = T{0};

    mat.m[3][0] = T{0};
    mat.m[3][1] = T{0};
    mat.m[3][2] = T{0};
    mat.m[3][3] = T{1};

    return mat;
}

template<std::size_t N, typename T>
requires (N >= 3)
tinyla::mat<N,T> tinyla::mat<N,T>::translation(const vec<N-1,T>& t) requires (N == 4)
{
    /**
     * | 1  0  0  tx |
     * | 0  1  0  ty |
     * | 0  0  1  tz |
     * | 0  0  0  1  |
     */
    mat<N,T> mat(mat_init::uninitialized);

    mat.m[0][0] = T{1};
    mat.m[0][1] = T{0};
    mat.m[0][2] = T{0};
    mat.m[0][3] = T{0};

    mat.m[1][0] = T{0};
    mat.m[1][1] = T{1};
    mat.m[1][2] = T{0};
    mat.m[1][3] = T{0};

    mat.m[2][0] = T{0};
    mat.m[2][1] = T{0};
    mat.m[2][2] = T{1};
    mat.m[2][3] = T{0};

    mat.m[3][0] = t.x();
    mat.m[3][1] = t.y();
    mat.m[3][2] = t.z();
    mat.m[3][3] = T{1};

    return mat;
}


template<std::size_t N, typename T>
requires (N >= 3)
tinyla::mat<N,T> tinyla::mat<N,T>::rotation(T angle, const vec<N-1,T>& axis) requires (N == 4)
{
    const T a = deg_to_rad(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    T len = x*x + y*y + z*z;
    if (!tinyla::close(len, T{1}) && !close_to_zero(len)) {
        len = std::sqrt(len);
        x = x / len;
        y = y / len;
        z = z / len;
    }
    T ic = T{1} - c;

    mat<N,T> mat(mat_init::uninitialized);

    mat.m[0][0] = x * x * ic + c;
    mat.m[0][1] = y * x * ic + z * s;
    mat.m[0][2] = x * z * ic - y * s;
    mat.m[0][3] = T{0};

    mat.m[1][0] = x * y * ic - z * s;
    mat.m[1][1] = y * y * ic + c;
    mat.m[1][2] = y * z * ic + x * s;
    mat.m[1][3] = T{0};

    mat.m[2][0] = x * z * ic + y * s;
    mat.m[2][1] = y * z * ic - x * s;
    mat.m[2][2] = z * z * ic + c;
    mat.m[2][3] = T{0};

    mat.m[3][0] = T{0};
    mat.m[3][1] = T{0};
    mat.m[3][2] = T{0};
    mat.m[3][3] = T{1};

    return mat;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N,T> tinyla::mat<N,T>::perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane) requires(N == 4)
{
    assert(nearPlane != farPlane);
    assert(aspectRatio != T{0});

    const T halfAngle = deg_to_rad(verticalAngle / 2);
    const T sine = std::sin(halfAngle);
    assert(sine != T{0});
    T cotan = std::cos(halfAngle) / sine;
    T clip = farPlane - nearPlane;

    mat<N,T> p(mat_init::uninitialized);

    p.m[0][0] = cotan / aspectRatio;
    p.m[0][1] = T{0};
    p.m[0][2] = T{0};
    p.m[0][3] = T{0};

    p.m[1][0] = T{0};
    p.m[1][1] = cotan;
    p.m[1][2] = T{0};
    p.m[1][3] = T{0};

    p.m[2][0] = T{0};
    p.m[2][1] = T{0};
    p.m[2][2] = -(nearPlane + farPlane) / clip;
    p.m[2][3] = T{-1};

    p.m[3][0] = T{0};
    p.m[3][1] = T{0};
    p.m[3][2] = -(2 * nearPlane * farPlane) / clip;
    p.m[3][3] = T{0};

    return p;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N,T> tinyla::mat<N,T>::operator*=(const mat<N,T>& other)
{
    const auto o = other; // prevent aliasing when &o == this
    *this = *this * o;
    return *this;
}

template<std::size_t N, typename T>
requires(N >= 3)
bool tinyla::mat<N,T>::close_to(mat<N,T> const& other)
{
    // A simple iterative algorithm but for column-major order.
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            if (!close(m[i][j], other.m[i][j])) {
                return false;
            }
        }
    }
    return true;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::mat<N,T> tinyla::operator*(const mat<N,T>& a, const mat<N,T>& b)
{
    auto c = tinyla::mat<N,T>{mat_init::uninitialized};
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            c.m[i][j] = T{0};
            for (size_t k = 0; k < N; ++k) {
                c.m[i][j] += a.m[k][j] * b.m[i][k];
            }
        }
    }
    return c;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::vec<N,T> tinyla::operator*(const vec<N,T>& v, const mat<N,T>& m)
{

}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::vec<N,T> tinyla::operator*(const mat<N,T>& m, const vec<N,T>& v)
{
    auto mv = tinyla::vec<N,T>{vec_init::uninitialized};
    for (size_t i = 0; i < N; ++i) {
        mv[i] = T{0};
        for (size_t j = 0; j < N; ++j) {
            mv[i] += m(j, i) * v[j];
        }
    }
    return mv;
}

#endif // TINYLA_MATRIX_H
