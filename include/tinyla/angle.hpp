#ifndef TINYLA_ANGLE_HPP
#define TINYLA_ANGLE_HPP

#include <numbers>

namespace tinyla {
    template<typename T>
    class angle {
    public:
        constexpr static angle from_radians(T radians)
        {
            return angle{radians};
        }
        constexpr static angle from_degrees(T degrees)
        {
            return angle{degrees * (std::numbers::pi_v<T> / 180)};
        }
        T radians() const
        {
            return m_radians;
        }
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
}

#endif // TINYLA_ANGLE_HPP
