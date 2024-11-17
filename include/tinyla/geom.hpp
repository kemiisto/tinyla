#ifndef TINYLA_GEOM_HPP
#define TINYLA_GEOM_HPP

#include <tinyla/mat.hpp>
#include <cstddef>
#include <numbers>

namespace tinyla {
    template<std::size_t N, typename T>
    requires(N >= 2)
    class vec;
}

namespace tinyla::geom {
    template<typename T>
    class angle {
    public:
        constexpr static angle from_radians(T radians);
        constexpr static angle from_degrees(T degrees);
        T radians() const { return m_radians; }
        // Unary minus (negation)
        constexpr angle operator-() noexcept { return angle{-m_radians}; }
    private:
        explicit constexpr angle(T r) : m_radians{r} {}
        T m_radians;
    };

    namespace literals {
        constexpr angle<float> operator ""_radf(long double n)
        {
            return angle<float>::from_radians(static_cast<float>(n));
        }
        constexpr angle<float> operator ""_degf(long double n)
        {
            return angle<float>::from_degrees(static_cast<float>(n));
        }

        constexpr angle<double> operator ""_radd(long double n)
        {
            return angle<double>::from_radians(static_cast<double>(n));
        }
        constexpr angle<double> operator ""_degd(long double n)
        {
            return angle<double>::from_degrees(static_cast<double>(n));
        }

        constexpr angle<long double> operator ""_radld(long double n)
        {
            return angle<long double>::from_radians(n);
        }
        constexpr angle<long double> operator ""_degld(long double n)
        {
            return angle<long double>::from_degrees(n);
        }
    }

    enum class handedness {
        left,
        right
    };

    enum class clip_volume {
        zero_to_one,        // Vulkan and Direct3D
        minus_one_to_one    // OpenGL
    };

    template<typename T>
    class frustum {
    public:
        constexpr frustum(angle<T> const& fov, T ar, T z_near, T z_far);
        angle<T> const& fov() const { return m_fov; }
        T ar() const { return m_ar; }
        T z_near() const { return m_z_near; }
        T z_far() const { return m_z_far; }
    private:
        angle<T> m_fov;
        T m_ar;
        T m_z_near;
        T m_z_far;
    };

    template<typename T>
    mat<4,T> perspective(frustum<T> const& frustum, handedness handedness, clip_volume clip_volume);

    template<typename T>
    vec<4, T> project(mat<4, T> const& m, vec<4, T> const& v);

    template<typename T>
    mat<4, T> scaling(vec<3, T> const& s);

    template<typename T>
    mat<4, T> translation(vec<3, T> const& t);

    template<typename T>
    mat<4, T> rotation(angle<T> const& angle, vec<3, T> const& axis);

    template<typename T>
    void pre_rotate(mat<4, T>& m, angle<T> const& angle, vec<3, T> const& axis);

    template<typename T>
    void post_rotate(mat<4, T>& m, const angle<T>& angle, const vec<3, T>& axis);

    template<typename T>
    void pre_translate(mat<4, T>& m, vec<3, T> const& t);

    template<typename T>
    void post_translate(mat<4, T>& m, const vec<3, T>& t);

    template<typename T>
    void pre_scale(mat<4, T>& m, vec<3, T> const& s);

    template<typename T>
    void post_scale(mat<4, T>& m, vec<3, T> const& s);

    namespace detail {
        template<typename T>
        mat<4,T> perspective_rh_mo(frustum<T> const& frustum);

        template<typename T>
        mat<4,T> perspective_rh_zo(frustum<T> const& frustum);

        template<typename T>
        void pre_rotate_x(tinyla::mat<4, T>& m, T c, T s);

        template<typename T>
        void post_rotate_x(tinyla::mat<4, T>& m, T c, T s);

        template<typename T>
        void pre_rotate_y(tinyla::mat<4, T>& m, T c, T s);

        template<typename T>
        void post_rotate_y(tinyla::mat<4, T>& m, T c, T s);

        template<typename T>
        void pre_rotate_z(tinyla::mat<4, T>& m, T c, T s);

        template<typename T>
        void post_rotate_z(tinyla::mat<4, T>& m, T c, T s);
    }
}

#include "geom.inl"

#endif // TINYLA_GEOM_HPP
