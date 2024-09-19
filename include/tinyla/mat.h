#ifndef TINYLA_MATRIX_H
#define TINYLA_MATRIX_H

#include <tinyla/vec.h>
#include <tinyla/util.h>
#include <array>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace tinyla
{
    enum class MatInit {
        Uninitialized,
        Identity,
        Diagonal
    };

    template<std::size_t N, typename T>
    requires(N >= 3)
    class Mat;

    template<std::size_t N, typename T>
    requires(N >= 3)
    Mat<N,T> operator*(const Mat<N,T>& a, const Mat<N,T>& b);

    template<std::size_t N, typename T>
    requires(N >= 3)
    class Mat
    {
    public:
        static Mat scaling(const Vec<N - 1,T>& s) requires (N == 4);
        static Mat translation(const Vec<N-1,T>& t) requires (N == 4);
        static Mat rotation(T angle, const Vec<N-1,T>& axis) requires (N == 4);
        static Mat perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane) requires(N == 4);

        explicit Mat(MatInit init, Vec<N,T> v = {});
        Mat(std::initializer_list<T> values);

        constexpr T& operator()(std::size_t i, std::size_t j);
        constexpr T operator()(std::size_t i, std::size_t j) const;

        Mat operator*=(const Mat& o);

        void setToIdentity();
        void setToDiagonal(Vec<N,T> const& v);

        void preScale(T s) requires (N == 4);
        void postScale(T s) requires (N == 4);

        void preScale(const Vec<N - 1,T>& s) requires (N == 4);
        void postScale(const Vec<N - 1,T>& s) requires (N == 4);

        void preTranslate(const Vec<N-1,T>& t) requires (N == 4);
        void postTranslate(const Vec<N-1,T>& t) requires (N == 4);

        void preRotate(T angle, const Vec<N-1,T>& axis) requires (N == 4);
        void postRotate(T angle, const Vec<N-1,T>& axis) requires (N == 4);

        T determinant() const requires (N == 4);
        Mat inverted() const requires (N == 4);

        constexpr T* data() noexcept;
        constexpr const T* data() const noexcept;

        bool closeTo(const Mat& other);

        friend Mat<N,T> operator* <>(const Mat<N,T>& a, const Mat<N,T>& b);
    private:
        T m[N][N]{};

        void preRotateX(T c, T s) requires (N == 4);
        void postRotateX(T c, T s) requires (N == 4);

        void preRotateY(T c, T s) requires (N == 4);
        void postRotateY(T c, T s) requires (N == 4);

        void preRotateZ(T c, T s) requires (N == 4);
        void postRotateZ(T c, T s) requires (N == 4);
    };

    using Mat3 = Mat<3,float>;
    using Mat4 = Mat<4,float>;
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::Mat<N,T>::Mat(tinyla::MatInit init, tinyla::Vec<N,T> v)
{
    switch (init) {
        case MatInit::Uninitialized:
            break;
        case MatInit::Identity:
            setToIdentity();
            break;
        case MatInit::Diagonal:
            setToDiagonal(v);
            break;
    }
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::Mat<N,T>::Mat(std::initializer_list<T> values)
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
constexpr T& tinyla::Mat<N,T>::operator()(std::size_t i, std::size_t j)
{
    return m[i][j];
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr T tinyla::Mat<N,T>::operator()(std::size_t i, std::size_t j) const
{
    return m[i][j];
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::Mat<N,T>::setToIdentity()
{
    auto i = tinyla::Vec<N,T>{};
    i.fill(T{1});
    setToDiagonal(i);
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::Mat<N,T>::setToDiagonal(Vec<N,T> const& v)
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
void tinyla::Mat<N,T>::preTranslate(const tinyla::Vec<N - 1,T>& t) requires (N == 4)
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
void tinyla::Mat<N,T>::postTranslate(const tinyla::Vec<N - 1,T>& t) requires (N == 4)
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
void tinyla::Mat<N,T>::preScale(T s) requires (N == 4)
{
    preScale({s, s, s});
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::Mat<N,T>::postScale(T s) requires (N == 4)
{
    postScale({s, s, s});
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::Mat<N,T>::preScale(const Vec<N - 1,T>& s) requires (N == 4)
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
void tinyla::Mat<N,T>::postScale(const Vec<N - 1,T>& s) requires (N == 4)
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
void tinyla::Mat<N,T>::preRotateX(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::postRotateX(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::preRotateY(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::postRotateY(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::preRotateZ(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::postRotateZ(T c, T s) requires (N == 4)
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
void tinyla::Mat<N,T>::preRotate(T angle, const tinyla::Vec<N - 1,T>& axis)  requires (N == 4)
{
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    if (x == T{0}) {
        if (y == T{0}) {
            if (z != T{0}) {
                preRotateZ(c, s);
                return;
            }
        } else if (z == T{0}) {
            preRotateY(c, s);
            return;
        }
    } else if (y == T{0} && z == T{0}) {
        preRotateX(c, s);
        return;
    }

    *this = Mat::rotation(angle, axis) * (*this);
}

template<std::size_t N, typename T>
requires(N >= 3)
void tinyla::Mat<N,T>::postRotate(T angle, const tinyla::Vec<N - 1,T>& axis) requires (N == 4)
{
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    if (x == T{0}) {
        if (y == T{0}) {
            if (z != T{0}) {
                postRotateZ(c, s);
                return;
            }
        } else if (z == T{0}) {
            postRotateY(c, s);
            return;
        }
    } else if (y == T{0} && z == T{0}) {
        postRotateX(c, s);
        return;
    }

    *this *= Mat::rotation(angle, axis);
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
T tinyla::Mat<N,T>::determinant() const requires (N == 4)
{
    return det4(m);
}

template<std::size_t N, typename T>
requires(N >= 3)
tinyla::Mat<N, T> tinyla::Mat<N,T>::inverted() const requires (N == 4)
{
    tinyla::Mat<N,T> inv(tinyla::MatInit::Uninitialized);

    T det = det4(m);
    if (closeToZero(det)) {
        return tinyla::Mat<N,T>(tinyla::MatInit::Identity);
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
constexpr T* tinyla::Mat<N,T>::data() noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires(N >= 3)
constexpr const T* tinyla::Mat<N,T>::data() const noexcept
{
    return *m;
}

template<std::size_t N, typename T>
requires (N >= 3)
tinyla::Mat<N, T> tinyla::Mat<N, T>::scaling(tinyla::Vec<N - 1, T> const& s) requires (N == 4)
{
    /**
     * | sx  0   0   0 |
     * | 0   sy  0   0 |
     * | 0   0   sz  0 |
     * | 0   0   0   1 |
     */
    tinyla::Mat<N,T> mat(tinyla::MatInit::Uninitialized);

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
tinyla::Mat<N,T> tinyla::Mat<N,T>::translation(const Vec<N-1,T>& t) requires (N == 4)
{
    /**
     * | 1  0  0  tx |
     * | 0  1  0  ty |
     * | 0  0  1  tz |
     * | 0  0  0  1  |
     */
    tinyla::Mat<N,T> mat(tinyla::MatInit::Uninitialized);

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
tinyla::Mat<N,T> tinyla::Mat<N,T>::rotation(T angle, const Vec<N-1,T>& axis) requires (N == 4)
{
    const T a = degreesToRadians(angle);
    const T c = std::cos(a);
    const T s = std::sin(a);

    auto x = axis.x();
    auto y = axis.y();
    auto z = axis.z();

    T len = x*x + y*y + z*z;
    if (!tinyla::close(len, T{1}) && !closeToZero(len)) {
        len = std::sqrt(len);
        x = x / len;
        y = y / len;
        z = z / len;
    }
    T ic = T{1} - c;

    tinyla::Mat<N,T> mat(tinyla::MatInit::Uninitialized);

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
tinyla::Mat<N,T> tinyla::Mat<N,T>::perspective(T verticalAngle, T aspectRatio, T nearPlane, T farPlane) requires(N == 4)
{
    assert(nearPlane != farPlane);
    assert(aspectRatio != T{0});

    const T halfAngle = degreesToRadians(verticalAngle / 2);
    const T sine = std::sin(halfAngle);
    assert(sine != T{0});
    T cotan = std::cos(halfAngle) / sine;
    T clip = farPlane - nearPlane;

    tinyla::Mat<N,T> p(tinyla::MatInit::Uninitialized);

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
tinyla::Mat<N,T> tinyla::Mat<N,T>::operator*=(const tinyla::Mat<N,T>& other)
{
    const auto o = other; // prevent aliasing when &o == this
    *this = *this * o;
    return *this;
}

template<std::size_t N, typename T>
requires(N >= 3)
bool tinyla::Mat<N,T>::closeTo(tinyla::Mat<N,T> const& other)
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
tinyla::Mat<N,T> tinyla::operator*(const Mat<N,T>& a, const Mat<N,T>& b)
{
    auto c = tinyla::Mat<N,T>(tinyla::MatInit::Uninitialized);
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < N; ++j) {
            c.m[i][j] = T{0};
            for (size_t k = 0; k < 4; ++k) {
                c.m[i][j] += a.m[k][j] * b.m[i][k];
            }
        }
    }
    return c;
}

#endif // TINYLA_MATRIX_H
