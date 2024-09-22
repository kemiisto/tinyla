#ifndef TINYLA_UTIL_H
#define TINYLA_UTIL_H

#include <algorithm>
#include <concepts>
#include <numbers>
#include <numeric>

namespace tinyla
{
    template<std::floating_point T>
    constexpr inline T deg_to_rad(T degrees)
    {
        return degrees * (std::numbers::pi_v<T> / 180);
    }

    template<std::floating_point T>
    constexpr inline T rad_to_deg(T radians)
    {
        return radians * (180 / std::numbers::pi_v<T>);
    }

    template<std::floating_point T>
    constexpr bool close(T n1, T n2)
    {
        return std::abs(n1 - n2) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(n1), std::abs(n2));
    }

    template<std::floating_point T>
    constexpr bool close_to_zero(T n)
    {
        return std::abs(n) <= std::numeric_limits<T>::epsilon();
    }
}

#endif // TINYLA_UTIL_H
