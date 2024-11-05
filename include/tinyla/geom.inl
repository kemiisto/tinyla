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
