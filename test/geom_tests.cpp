#include <tinyla/geom.hpp>
#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include "compare.hpp"
#include <array>

using namespace tinyla::literals;

TEST_CASE("mat4 perspective", "[mat4]")
{
    const auto m = tinyla::geom::perspective(
        60.0_degf, 1.0f, 0.1f, 1000.0f,
        tinyla::geom::frustum::right_handed, tinyla::geom::clip_volume::minus_one_to_one);
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.732051f, 0.000000f,  0.000000f,  0.000000f,
        0.000000f, 1.732051f,  0.000000f,  0.000000f,
        0.000000f, 0.000000f, -1.000200f, -0.200020f,
        0.000000f, 0.000000f, -1.000000f,  0.000000f
    };
    compare(m, a);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
