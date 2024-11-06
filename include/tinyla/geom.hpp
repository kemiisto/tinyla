#ifndef TINYLA_GEOM_HPP
#define TINYLA_GEOM_HPP

#include <tinyla/mat.hpp>

namespace tinyla::geom {
    enum class frustum {
        left_handed,
        right_handed
    };

    enum class clip_volume {
        zero_to_one,        // Vulkan and Direct3D
        minus_one_to_one    // OpenGL
    };

    template<typename T>
    mat<4,T> perspective(angle<T> const& vertical_fov, T aspect_ratio, T z_near, T z_far, frustum frustum, clip_volume clip_volume);

    template<typename T>
    vec<2, T> project(mat<4, T> const& m, vec<4, T> const& v);

    template<typename T>
    mat<4, T> scaling(vec<3, T> const& s);

    template<typename T>
    mat<4, T> translation(vec<3, T> const& t);

    template<typename T>
    mat<4, T> rotation(angle<T> const& angle, vec<3, T> const& axis);

    template<typename T>
    void pre_rotate(mat<4, T> const& m, angle<T> const& angle, vec<3, T> const& axis);

    template<typename T>
    void post_rotate(mat<4, T> const& m, const angle<T>& angle, const vec<3, T>& axis);

    template<typename T>
    void pre_translate(mat<4, T>& m, vec<3, T> const& t);

    template<typename T>
    void post_translate(mat<4, T>& m, const vec<3, T>& t);

    template<typename T>
    void pre_scale(mat<4, T>& m, T s)
    {
        pre_scale({s, s, s});
    }

    template<typename T>
    void post_scale(mat<4, T>& m, T s)
    {
        post_scale({s, s, s});
    }

    template<typename T>
    void pre_scale(mat<4, T>& m, vec<3, T> const& s);

    template<typename T>
    void post_scale(mat<4, T>& m, vec<3, T> const& s);

    namespace detail {
        template<typename T>
        mat<4,T> perspective_rh_mo(angle<T> const& vertical_fov, T aspect_ratio, T z_near, T z_far);

        template<typename T>
        mat<4,T> perspective_rh_zo(const angle<T>& vertical_fov, T aspect_ratio, T z_near, T z_far);

        template<typename T>
        void pre_rotate_x(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * | 1   0   0   0 |   | m00   m01   m02   m03 |   | m00           m01           m02           m03         |
             * | 0   c  -s   0 |   | m10   m11   m12   m13 |   | m10*c-m20*s   m11*c-m21*s   m12*c-m22*s   m13*c-m23*s |
             * | 0   s   c   0 | * | m20   m21   m22   m23 | = | m20*c+m10*s   m21*c+m11*s   m22*c+m12*s   m23*c+m13*s |
             * | 0   0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
             */
            T tmp;
            m(1, 0) = (tmp = m(1, 0)) * c - m(2, 0) * s;    m(2, 0) = m(2, 0) * c + tmp * s;
            m(1, 1) = (tmp = m(1, 1)) * c - m(2, 1) * s;    m(2, 1) = m(2, 1) * c + tmp * s;
            m(1, 2) = (tmp = m(1, 2)) * c - m(2, 2) * s;    m(2, 2) = m(2, 2) * c + tmp * s;
            m(1, 3) = (tmp = m(1, 3)) * c - m(2, 3) * s;    m(2, 3) = m(2, 3) * c + tmp * s;
        }

        template<typename T>
        void post_rotate_x(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * | m00   m01   m02   m03 |   | 1   0   0   0 |   | m00    m01*c + m02*s    -m01*s + m02*c   m03 |
             * | m10   m11   m12   m13 |   | 0   c  -s   0 |   | m10    m11*c + m12*s    -m11*s + m12*c   m13 |
             * | m20   m21   m22   m23 | * | 0   s   c   0 | = | m20    m21*c + m22*s    -m21*s + m22*c   m23 |
             * | m30   m31   m32   m33 |   | 0   0   0   1 |   | m30    m31*c + m32*s    -m31*s + m32*c   m33 |
             */

            T tmp;
            m(0, 1) = (tmp = m(0, 1)) * c + m(0, 2) * s;    m(0, 2) = -tmp * s + m(0, 2) * c;
            m(1, 1) = (tmp = m(1, 1)) * c + m(1, 2) * s;    m(1, 2) = -tmp * s + m(1, 2) * c;
            m(2, 1) = (tmp = m(2, 1)) * c + m(2, 2) * s;    m(2, 2) = -tmp * s + m(2, 2) * c;
            m(3, 1) = (tmp = m(3, 1)) * c + m(3, 2) * s;    m(3, 2) = -tmp * s + m(3, 2) * c;
        }

        template<typename T>
        void pre_rotate_y(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * |  c   0   s   0 |   | m00   m01   m02   m03 |   | m00*c+m20*s   m01*c+m21*s   m02*c+m22*s   m03*c+m23*s |
             * |  0   1   0   0 |   | m10   m11   m12   m13 |   | m10           m11           m12           m13         |
             * | -s   0   c   0 | * | m20   m21   m22   m23 | = | m20*c-m00*s   m21*c-m01*s   m22*c-m02*s   m23*c-m03*s |
             * |  0   0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
             */
            T tmp;
            m(0, 0) = (tmp = m(0, 0)) * c + m(2, 0) * s;    m(2, 0) = m(2, 0) * c - tmp * s;
            m(0, 1) = (tmp = m(0, 1)) * c + m(2, 1) * s;    m(2, 1) = m(2, 1) * c - tmp * s;
            m(0, 2) = (tmp = m(0, 2)) * c + m(2, 2) * s;    m(2, 2) = m(2, 2) * c - tmp * s;
            m(0, 3) = (tmp = m(0, 3)) * c + m(2, 3) * s;    m(2, 3) = m(2, 3) * c - tmp * s;
        }

        template<typename T>
        void post_rotate_y(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * | m00   m01   m02   m03 |   |  c   0   s   0 |   | m00*c - m02*s   m01   m00*s + m02*c   m03 |
             * | m10   m11   m12   m13 |   |  0   1   0   0 |   | m10*c - m12*s   m11   m10*s + m12*c   m13 |
             * | m20   m21   m22   m23 | * | -s   0   c   0 | = | m20*c - m22*s   m21   m20*s + m22*c   m23 |
             * | m30   m31   m32   m33 |   |  0   0   0   1 |   | m30*c - m32*s   m31   m30*s + m32*c   m33 |
             */
            T tmp;
            m(0, 0) = (tmp = m(0, 0)) * c - m(0, 2) * s;    m(0, 2) = tmp * s + m(0, 2) * c;
            m(1, 0) = (tmp = m(1, 0)) * c - m(1, 2) * s;    m(1, 2) = tmp * s + m(1, 2) * c;
            m(2, 0) = (tmp = m(2, 0)) * c - m(2, 2) * s;    m(2, 2) = tmp * s + m(2, 2) * c;
            m(3, 0) = (tmp = m(3, 0)) * c - m(3, 2) * s;    m(3, 2) = tmp * s + m(3, 2) * c;
        }

        template<typename T>
        void pre_rotate_z(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * | c   -s   0   0 |   | m00   m01   m02   m03 |   | m00*c-m10*s   m01*c-m11*s   m02*c-m12*s   m03*c-m13*s |
             * | s    c   0   0 |   | m10   m11   m12   m13 |   | m10*c+m00*s   m11*c+m01*s   m12*c+m02*s   m13*c+m03*s |
             * | 0    0   1   0 | * | m20   m21   m22   m23 | = | m20           m21           m22           m23         |
             * | 0    0   0   1 |   | m30   m31   m32   m33 |   | m30           m31           m32           m33         |
             */
            T tmp;
            m(0, 0) = (tmp = m(0, 0)) * c - m(1, 0) * s;  m(1, 0) = m(1, 0) * c + tmp * s;
            m(0, 1) = (tmp = m(0, 1)) * c - m(1, 1) * s;  m(1, 1) = m(1, 1) * c + tmp * s;
            m(0, 2) = (tmp = m(0, 2)) * c - m(1, 2) * s;  m(1, 2) = m(1, 2) * c + tmp * s;
            m(0, 3) = (tmp = m(0, 3)) * c - m(1, 3) * s;  m(1, 3) = m(1, 3) * c + tmp * s;
        }

        template<typename T>
        void post_rotate_z(tinyla::mat<4, T>& m, T c, T s)
        {
            /**
             * | m00   m01   m02   m03 |   | c   -s   0   0 |   | m00*c + m01*s   -m00*s - m01*c   m02   m03 |
             * | m10   m11   m12   m13 |   | s    c   0   0 |   | m10*c + m11*s   -m10*s - m11*c   m12   m13 |
             * | m20   m21   m22   m23 | * | 0    0   1   0 | = | m20*c + m21*s   -m20*s - m21*c   m22   m23 |
             * | m30   m31   m32   m33 |   | 0    0   0   1 |   | m30*c + m31*s   -m30*s - m31*c   m32   m33 |
             */

            T tmp;
            m(0, 0) = (tmp = m(0, 0)) * c + m(0, 1) * s;    m(0, 1) = -tmp * s + m(0, 1) * c;
            m(1, 0) = (tmp = m(1, 0)) * c + m(1, 1) * s;    m(1, 1) = -tmp * s + m(1, 1) * c;
            m(2, 0) = (tmp = m(2, 0)) * c + m(2, 1) * s;    m(2, 1) = -tmp * s + m(2, 1) * c;
            m(3, 0) = (tmp = m(3, 0)) * c + m(3, 1) * s;    m(3, 1) = -tmp * s + m(3, 1) * c;
        }
    }
}

#include "geom.inl"

#endif // TINYLA_GEOM_HPP
