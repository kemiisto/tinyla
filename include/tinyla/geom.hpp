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

    namespace detail {
        template<typename T>
        mat<4,T> perspective_rh_mo(angle<T> const& vertical_fov, T aspect_ratio, T z_near, T z_far);

        template<typename T>
        mat<4,T> perspective_rh_zo(const angle<T>& vertical_fov, T aspect_ratio, T z_near, T z_far);
    }
}

#include "geom.inl"

#endif // TINYLA_GEOM_HPP
