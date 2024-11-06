#ifndef TINYLA_GEOM_INL
#define TINYLA_GEOM_INL

/**
* rh - right-handed
* lh - left-handed
* mo - minus one to one (-1 to 1)
* zo - zero to one (0 to 1)
*/

namespace tinyla::geom {
    template<typename T>
    mat<4,T> perspective(angle<T> const& vertical_fov, T aspect_ratio, T z_near, T z_far, frustum frustum, clip_volume clip_volume)
    {
        auto p = mat<4, T>{mat_init::uninitialized};
        if (clip_volume == clip_volume::minus_one_to_one) {
            p = detail::perspective_rh_mo(vertical_fov, aspect_ratio, z_near, z_far);
        } else {
            p = detail::perspective_rh_zo(vertical_fov, aspect_ratio, z_near, z_far);
        }
        if (frustum == frustum::left_handed) {
            p(2, 2) = -p(2, 2);
            p(3, 2) = -p(3, 2);
        }
        return p;
    }

    template<typename T>
    vec<2, T> project(mat<4, T> const& m, vec<4, T> const& v)
    {
        auto result =  m * v;
        if (result.w() != T{0}) {
            result /= result.w();
        }
        return {result.x(), result.y()};
    }

    template<typename T>
    tinyla::mat<4, T> scaling(vec<3, T> const& s)
    {
        /**
         * | sx  0   0   0 |
         * | 0   sy  0   0 |
         * | 0   0   sz  0 |
         * | 0   0   0   1 |
         */
        auto m = mat<4, T>{mat_init::zero};
        m(0, 0) = s.x();
        m(1, 1) = s.y();
        m(2, 2) = s.z();
        m(3, 3) = T{1};
        return m;
    }

    template<typename T>
    tinyla::mat<4, T> translation(vec<3, T> const& t)
    {
        /**
         * | 1  0  0  tx |
         * | 0  1  0  ty |
         * | 0  0  1  tz |
         * | 0  0  0  1  |
         */
        auto m = mat<4, T>{mat_init::identity};
        m(0, 3) = t.x();
        m(1, 3) = t.y();
        m(2, 3) = t.z();
        return m;
    }


    template<typename T>
    tinyla::mat<4, T> rotation(angle<T> const& angle, tinyla::vec<3, T> const& axis)
    {
        T const a = angle.radians();
        T const c = std::cos(a);
        T const s = std::sin(a);

        auto x = axis.x();
        auto y = axis.y();
        auto z = axis.z();

        T len = x * x + y * y + z * z;
        if (!tinyla::close(len, T{ 1 }) && !close_to_zero(len)) {
            len = std::sqrt(len);
            x = x / len;
            y = y / len;
            z = z / len;
        }
        T ic = T{ 1 } - c;

        auto m = mat<4, T>{mat_init::uninitialized};

        m(0, 0) = x * x * ic + c;
        m(1, 0) = y * x * ic + z * s;
        m(2, 0) = x * z * ic - y * s;
        m(3, 0) = T{ 0 };

        m(0, 1) = x * y * ic - z * s;
        m(1, 1) = y * y * ic + c;
        m(2, 1) = y * z * ic + x * s;
        m(3, 1) = T{ 0 };

        m(0, 2) = x * z * ic + y * s;
        m(1, 2) = y * z * ic - x * s;
        m(2, 2) = z * z * ic + c;
        m(3, 2) = T{ 0 };

        m(0, 3) = T{0};
        m(1, 3) = T{0};
        m(2, 3) = T{0};
        m(3, 3) = T{ 1 };

        return m;
    }

    template<typename T>
    void pre_translate(mat<4, T>& m, vec<3, T> const& t)
    {
        /**
         * | 1  0  0  tx |   | m00   m01   m02   m03 |   | m00 + m30*tx   m01 + m31*tx   m02 + m32*tx   m03 + m33*tx |
         * | 0  1  0  ty |   | m10   m11   m12   m13 |   | m10 + m30*ty   m11 + m31*ty   m12 + m32*ty   m13 + m33*ty |
         * | 0  0  1  tz | * | m20   m21   m22   m23 | = | m20 + m30*ty   m21 + m31*ty   m22 + m32*ty   m23 + m33*ty |
         * | 0  0  0  1  |   | m30   m31   m32   m33 |   | m30            m31            m32            m33          |
         *
         * All columns are changing.
         */
        m(0, 0) += m(3, 0) * t.x();
        m(1, 0) += m(3, 0) * t.y();
        m(2, 0) += m(3, 0) * t.z();

        m(0, 1) += m(3, 1) * t.x();
        m(1, 1) += m(3, 1) * t.y();
        m(2, 1) += m(3, 1) * t.z();

        m(0, 2) += m(3, 2) * t.x();
        m(1, 2) += m(3, 2) * t.y();
        m(2, 2) += m(3, 2) * t.z();

        m(0, 3) += m(3, 3) * t.x();
        m(1, 3) += m(3, 3) * t.y();
        m(2, 3) += m(3, 3) * t.z();
    }

    template<typename T>
    void post_translate(mat<4, T>& m, const vec<3, T>& t)
    {
        /**
         * | m00   m01   m02   m03 |   | 1  0  0  tx |   | m00   m01   m02   m00*tx + m01*ty + m02*tz + m03 |
         * | m10   m11   m12   m13 |   | 0  1  0  ty |   | m10   m11   m12   m10*tx + m11*ty + m12*tz + m13 |
         * | m20   m21   m22   m23 | * | 0  0  1  tz | = | m20   m21   m22   m20*tx + m21*ty + m22*tz + m23 |
         * | m30   m31   m32   m33 |   | 0  0  0  1  |   | m30   m31   m32   m30*tx + m31*ty + m32*tz + m33 |
         *
         * Only the last column is changing.
         */
        m(0, 3) += m(0, 0) * t.x() + m(0, 1) * t.y() + m(0, 2) * t.z();
        m(1, 3) += m(1, 0) * t.x() + m(1, 1) * t.y() + m(1, 2) * t.z();
        m(2, 3) += m(2, 0) * t.x() + m(2, 1) * t.y() + m(2, 2) * t.z();
        m(3, 3) += m(3, 0) * t.x() + m(3, 1) * t.y() + m(3, 2) * t.z();
    }

    template<typename T>
    void pre_scale(mat<4, T>& m, vec<3, T> const& s)
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
        m(0, 0) *= s.x();
        m(1, 0) *= s.y();
        m(2, 0) *= s.z();

        m(0, 1) *= s.x();
        m(1, 1) *= s.y();
        m(2, 1) *= s.z();

        m(0, 2) *= s.x();
        m(1, 2) *= s.y();
        m(2, 2) *= s.z();

        m(0, 3) *= s.x();
        m(1, 3) *= s.y();
        m(2, 3) *= s.z();
    }

    template<typename T>
    void post_scale(mat<4, T>& m, vec<3, T> const& s)
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
        m(0, 0) *= s.x();
        m(1, 0) *= s.x();
        m(2, 0) *= s.x();
        m(3, 0) *= s.x();

        m(0, 1) *= s.y();
        m(1, 1) *= s.y();
        m(2, 1) *= s.y();
        m(3, 1) *= s.y();

        m(0, 2) *= s.z();
        m(1, 2) *= s.z();
        m(2, 2) *= s.z();
        m(3, 2) *= s.z();
    }

    template<typename T>
    void pre_rotate(mat<4, T>& m, angle<T> const& angle, vec<3, T> const& axis)
    {
        T const a = angle.radians();
        T const c = std::cos(a);
        T const s = std::sin(a);

        auto x = axis.x();
        auto y = axis.y();
        auto z = axis.z();

        if (x == T{0}) {
            if (y == T{0}) {
                if (z != T{0}) {
                    detail::pre_rotate_z(m, c, s);
                    return;
                }
            }
            else if (z == T{0}) {
                detail::pre_rotate_y(m, c, s);
                return;
            }
        }
        else if (y == T{0} && z == T{0}) {
            detail::pre_rotate_x(m, c, s);
            return;
        }

        m = rotation(angle, axis) * m;
    }

    template<typename T>
    void post_rotate(mat<4, T>& m, const angle<T>& angle, const vec<3, T>& axis)
    {
        const T a = angle.radians();
        const T c = std::cos(a);
        const T s = std::sin(a);

        auto x = axis.x();
        auto y = axis.y();
        auto z = axis.z();

        if (x == T{0}) {
            if (y == T{0}) {
                if (z != T{0}) {
                    detail::post_rotate_z(m, c, s);
                    return;
                }
            }
            else if (z == T{0}) {
                detail::post_rotate_y(m, c, s);
                return;
            }
        }
        else if (y == T{0} && z == T{0}) {
            detail::post_rotate_x(m, c, s);
            return;
        }

        m *= rotation(angle, axis);
    }

    namespace detail {
        template<typename T>
        mat<4,T> perspective_rh_mo(const angle<T>& vertical_fov, T aspect_ratio, T z_near, T z_far)
        {
            assert(z_near != z_far);
            assert(aspect_ratio != T{0});

            T const half_angle = vertical_fov.radians() / T{2};
            T const sin = std::sin(half_angle);
            assert(sin != T{0});
            T const cot = std::cos(half_angle) / sin;
            T const clip = z_far - z_near;

            auto p = mat<4, T>{mat_init::zero};
            p(0, 0) = cot / aspect_ratio;
            p(1, 1) = cot;
            p(2, 2) = -(z_far + z_near) / clip;
            p(3, 2) = -T{1};
            p(2, 3) = -(T{2} * z_far * z_near) / clip;
            return p;
        }

        template<typename T>
        mat<4,T> perspective_rh_zo(const angle<T>& vertical_fov, T aspect_ratio, T z_near, T z_far)
        {
            assert(z_near != z_far);
            assert(aspect_ratio != T{0});

            T const half_angle = vertical_fov.radians() / T{2};
            T const sin = std::sin(half_angle);
            assert(sin != T{0});
            T const cot = std::cos(half_angle) / sin;
            T const clip = z_far - z_near;

            auto p = mat<4, T>{mat_init::zero};
            p(0, 0) = cot / aspect_ratio;
            p(1, 1) = cot;
            p(2, 2) = -z_far / clip;
            p(3, 2) = -T{1};
            p(2, 3) = -(z_far * z_near) / clip;
            return p;
        }
    }
}

#endif // TINYLA_GEOM_INL