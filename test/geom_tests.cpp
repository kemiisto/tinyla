#include <tinyla/geom.hpp>
#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include "compare.hpp"
#include "data.hpp"
#include <array>

using namespace tinyla::literals;

TEST_CASE("mat4 perspective", "[mat4]")
{
    const auto frustum = tinyla::geom::frustum{60.0_degf, 1.0f, 0.1f, 1000.0f};
    const auto m = tinyla::geom::perspective(frustum,
        tinyla::geom::handedness::right, tinyla::geom::clip_volume::minus_one_to_one);
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.732051f, 0.000000f,  0.000000f,  0.000000f,
        0.000000f, 1.732051f,  0.000000f,  0.000000f,
        0.000000f, 0.000000f, -1.000200f, -0.200020f,
        0.000000f, 0.000000f, -1.000000f,  0.000000f
    };
    compare(m, a);
}

TEST_CASE("mat4 scaling", "[mat4]")
{
    const auto m = tinyla::geom::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        2.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 3.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 4.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    compare(m, a);
}

TEST_CASE("mat4 pre_scale", "[mat4]")
{
    auto m = unique;
    tinyla::geom::pre_scale(m, tinyla::vec3f{2.0f, 3.0f, 4.0f});

    auto const scaling_matrix = tinyla::geom::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
    auto const matrix = scaling_matrix * unique;
    
    compare(m, matrix);
}

TEST_CASE("mat4 post_scale", "[mat4]")
{
    auto m = unique;
    tinyla::geom::post_scale(m, tinyla::vec3f{2.0f, 3.0f, 4.0f});

    auto const scaling_matrix = tinyla::geom::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
    auto const matrix = unique * scaling_matrix;

    compare(m, matrix);
}

TEST_CASE("mat4 translation", "[mat4]")
{
    const auto m = tinyla::geom::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 2.0f,
            0.0f, 0.0f, 1.0f, 3.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };
    compare(m, a);
}

TEST_CASE("mat4 pre_translate", "[mat4]")
{
    auto m = unique;
    tinyla::geom::pre_translate(m, tinyla::vec3f{1.0f, 2.0f, 3.0f});

    const auto translation_matrix = tinyla::geom::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
    const auto matrix = translation_matrix * unique;

    compare(m, matrix);
}

TEST_CASE("mat4 post_translate", "[mat4]")
{
    auto m = unique;
    tinyla::geom::post_translate(m, tinyla::vec3f{1.0f, 2.0f, 3.0f});

    const auto translation_matrix = tinyla::geom::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
    const auto matrix = unique * translation_matrix;

    compare(m, matrix);
}

TEST_CASE("mat4 pre_rotate around x-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::pre_rotate(m, 45.0_degf, {1.0f, 0.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.000000f, 2.000000f, 3.000000f, 4.000000f,
            -2.828427f, -2.828427f, -2.828427f, -2.828427f,
            9.899494f, 11.313708f, 12.727922f, 14.142136f,
            13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around x-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::post_rotate(m, 45.0_degf, tinyla::vec3f{1.0f, 0.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.000000f, 3.535534f, 0.707107f, 4.000000f,
            5.000000f, 9.192389f, 0.707107f, 8.000000f,
            9.000000f, 14.849242f, 0.707107f, 12.000000f,
            13.000000f, 20.506096f, 0.707107f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 pre_rotate around y-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::pre_rotate(m, 45.0_degf, {0.0f, 1.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        7.071067f, 8.485281f, 9.899494f, 11.313708f,
            5.000000f, 6.000000f, 7.000000f, 8.000000f,
            5.656854f, 5.656854f, 5.656854f, 5.656854f,
            13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around y-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::post_rotate(m, 45.0_degf, {0.0f, 1.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        -1.414213f, 2.000000f, 2.828427f, 4.000000f,
            -1.414214f, 6.000000f, 8.485281f, 8.000000f,
            -1.414214f, 10.000000f, 14.142136f, 12.000000f,
            -1.414214f, 14.000000f, 19.798988f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 pre_rotate around z-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::pre_rotate(m, 45.0_degf, {0.0f, 0.0f, 1.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        -2.828427f, -2.828427f, -2.828427f, -2.828427f,
            4.242640f, 5.656854f, 7.071068f, 8.485281f,
            9.000000f, 10.000000f, 11.000000f, 12.000000f,
            13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around z-axis", "[mat4]")
{
    auto m = unique;
    tinyla::geom::post_rotate(m, 45.0_degf, {0.0f, 0.0f, 1.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        2.121320f, 0.707107f, 3.000000f, 4.000000f,
            7.778174f, 0.707107f, 7.000000f, 8.000000f,
            13.435028f, 0.707107f, 11.000000f, 12.000000f,
            19.091883f, 0.707108f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
