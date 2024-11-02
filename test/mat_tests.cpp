#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include "compare.hpp"
#include <array>

const auto zero = tinyla::mat4f {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
};

const auto unique = tinyla::mat4f {
     1.0f,  2.0f,  3.0f,  4.0f,
     5.0f,  6.0f,  7.0f,  8.0f,
     9.0f, 10.0f, 11.0f, 12.0f,
    13.0f, 14.0f, 15.0f, 16.0f
};

const auto identity = tinyla::mat4f {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f
};

template<std::size_t N, typename T>
void compare(const tinyla::mat<N,T>& m, const std::array<std::array<T,N>,N>& a)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(m[i, j] == Catch::Approx(a[i][j]));
        }
    }
}

template<std::size_t N, typename T>
void compare(const tinyla::mat<N,T>& m1, const tinyla::mat<N,T>& m2)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(m1[i, j] == Catch::Approx(m2[i, j]));
        }
    }
}

TEST_CASE("Mat3 is constructed from initializer list", "[Mat3]")
{
    const auto m = tinyla::mat3f {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };
    constexpr auto a = std::array<std::array<float,3>,3> {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };
    compare(m, a);
}

TEST_CASE("mat4 is constructed from initializer list", "[mat4]")
{
    const auto m = tinyla::mat4f {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };
    compare(m, a);
}

TEST_CASE("mat4 is constructed as identity matrix", "[mat4]")
{
    const auto m = tinyla::mat4f(tinyla::mat_init::identity);
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    };
    compare(m, a);
}

TEST_CASE("mat4 data", "[mat4]")
{
    const auto m = tinyla::mat4f {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };
    constexpr auto a = std::array<float, 4*4> {
        0.0f, 1.0f, 2.0f, 3.0f,
        0.1f, 1.1f, 2.1f, 3.1f,
        0.2f, 1.2f, 2.2f, 3.2f,
        0.3f, 1.3f, 2.3f, 3.3f
    };
    auto* data = m.data();
    for (std::size_t i = 0; i < 4*4; ++i) {
        REQUIRE(data[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("mat4 scaling", "[mat4]")
{
    const auto m = tinyla::mat4f::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
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
    auto pre_scaled = unique;
    pre_scaled.pre_scale(tinyla::vec3f{2.0f, 3.0f, 4.0f});

    const auto scaling_matrix = tinyla::mat4f::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
    const auto matrix = scaling_matrix * unique;

    compare(pre_scaled, matrix);
}

TEST_CASE("mat4 post_scale", "[mat4]")
{
    auto post_scaled = unique;
    post_scaled.post_scale(tinyla::vec3f{2.0f, 3.0f, 4.0f});

    const auto scaling_matrix = tinyla::mat4f::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
    const auto matrix = unique * scaling_matrix;

    compare(post_scaled, matrix);
}

TEST_CASE("mat4 translation", "[mat4]")
{
    const auto m = tinyla::mat4f::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
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
    auto pre_translated = unique;
    pre_translated.pre_translate(tinyla::vec3f{1.0f, 2.0f, 3.0f});

    const auto translation_matrix = tinyla::mat4f::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
    const auto matrix = translation_matrix * unique;

    compare(pre_translated, matrix);
}

TEST_CASE("mat4 post_translate", "[mat4]")
{
    auto post_translated = unique;
    post_translated.post_translate(tinyla::vec3f{1.0f, 2.0f, 3.0f});

    const auto translation_matrix = tinyla::mat4f::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
    const auto matrix = unique * translation_matrix;

    compare(post_translated, matrix);
}

TEST_CASE("mat4 pre_rotate around x-axis", "[mat4]")
{
    auto m = unique;
    m.pre_rotate(45.0f, {1.0f, 0.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
         1.000000f,  2.000000f,  3.000000f,  4.000000f,
        -2.828427f, -2.828427f, -2.828427f, -2.828427f,
         9.899494f, 11.313708f, 12.727922f, 14.142136f,
        13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around x-axis", "[mat4]")
{
    auto m = unique;
    m.post_rotate(45.0f, tinyla::vec3f{1.0f, 0.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
         1.000000f,  3.535534f, 0.707107f,  4.000000f,
         5.000000f,  9.192389f, 0.707107f,  8.000000f,
         9.000000f, 14.849242f, 0.707107f, 12.000000f,
        13.000000f, 20.506096f, 0.707107f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 pre_rotate around y-axis", "[mat4]")
{
    auto m = unique;
    m.pre_rotate(45.0f, {0.0f, 1.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
         7.071067f,  8.485281f,  9.899494f, 11.313708f,
         5.000000f,  6.000000f,  7.000000f,  8.000000f,
         5.656854f,  5.656854f,  5.656854f,  5.656854f,
        13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around y-axis", "[mat4]")
{
    auto m = unique;
    m.post_rotate(45.0f, {0.0f, 1.0f, 0.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        -1.414213f,  2.000000f,  2.828427f,  4.000000f,
        -1.414214f,  6.000000f,  8.485281f,  8.000000f,
        -1.414214f, 10.000000f, 14.142136f, 12.000000f,
        -1.414214f, 14.000000f, 19.798988f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 pre_rotate around z-axis", "[mat4]")
{
    auto m = unique;
    m.pre_rotate(45.0f, {0.0f, 0.0f, 1.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
        -2.828427f, -2.828427f, -2.828427f, -2.828427f,
         4.242640f,  5.656854f,  7.071068f,  8.485281f,
         9.000000f, 10.000000f, 11.000000f, 12.000000f,
        13.000000f, 14.000000f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("mat4 post_rotate around z-axis", "[mat4]")
{
    auto m = unique;
    m.post_rotate(45.0f, {0.0f, 0.0f, 1.0f});

    constexpr auto a = std::array<std::array<float, 4>, 4> {
         2.121320f, 0.707107f,  3.000000f,  4.000000f,
         7.778174f, 0.707107f,  7.000000f,  8.000000f,
        13.435028f, 0.707107f, 11.000000f, 12.000000f,
        19.091883f, 0.707108f, 15.000000f, 16.000000f
    };

    compare(m, a);
}

TEST_CASE("operator*=", "[mat4]")
{
    auto m = zero;
    m *= zero;
    REQUIRE(m.close_to(zero));

    m = zero;
    m *= unique;
    REQUIRE(m.close_to(zero));

    m = unique;
    m *= zero;
    REQUIRE(m.close_to(zero));

    m = identity;
    m *= unique;
    REQUIRE(m.close_to(unique));

    m = unique;
    m *= identity;
    REQUIRE(m.close_to(unique));
}

TEST_CASE("operator*", "[mat4]")
{
    auto m = zero * zero;
    REQUIRE(m.close_to(zero));

    m = zero * unique;
    REQUIRE(m.close_to(zero));

    m = unique * zero;
    REQUIRE(m.close_to(zero));

    m = identity * unique;
    REQUIRE(m.close_to(unique));

    m = unique * identity;
    REQUIRE(m.close_to(unique));
}

TEST_CASE("mat4 perspective", "[mat4]")
{
    const auto m = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
    constexpr auto a = std::array<std::array<float, 4>, 4> {
        1.732051f, 0.000000f,  0.000000f,  0.000000f,
        0.000000f, 1.732051f,  0.000000f,  0.000000f,
        0.000000f, 0.000000f, -1.000200f, -0.200020f,
        0.000000f, 0.000000f, -1.000000f,  0.000000f
    };
    compare(m, a);
}

TEST_CASE("det2")
{
    constexpr float a[][4] = {
         1.0f,  2.0f,  3.0f,  4.0f,
         5.0f,  6.0f,  7.0f,  8.0f,
         9.0f, 10.0f, 11.0f, 12.0f,
        13.0f, 14.0f, 15.0f, 16.0f
    };
    REQUIRE(det2(a, 0, 1, 0, 1) == Catch::Approx( -4.0f));
    REQUIRE(det2(a, 1, 2, 1, 2) == Catch::Approx( -4.0f));
    REQUIRE(det2(a, 2, 3, 2, 3) == Catch::Approx( -4.0f));
    REQUIRE(det2(a, 0, 3, 0, 3) == Catch::Approx(-36.0f));
}

TEST_CASE("det3")
{
    constexpr float a[][4] = {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
       13.0f, 14.0f, 15.0f, 16.0f
   };
    REQUIRE(det3(a, 0, 1, 2, 0, 1, 2) == Catch::Approx(0.0f));
    REQUIRE(det3(a, 1, 2, 3, 1, 2, 3) == Catch::Approx(0.0f));
    REQUIRE(det3(a, 0, 2, 3, 0, 2, 3) == Catch::Approx(0.0f));
}

TEST_CASE("det4")
{
    constexpr float a[][4] = {
        1.0f,  2.0f,  3.0f,  4.0f,
        5.0f,  6.0f,  7.0f,  8.0f,
        9.0f, 10.0f, 11.0f, 12.0f,
       13.0f, 14.0f, 15.0f, 16.0f
   };
    REQUIRE(det4(a) == Catch::Approx(0.0f));
}

TEST_CASE("mat4 determinant", "[mat4]")
{
    {
        const auto m = tinyla::mat4f(tinyla::mat_init::identity);
        REQUIRE(m.determinant() == Catch::Approx(1.0f));
    }
    {
        const auto m = unique;
        REQUIRE(m.determinant() == Catch::Approx(0.0f));
    }
    {
        auto m = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
        REQUIRE(m.determinant() == Catch::Approx(-0.60006f));
    }
}

TEST_CASE("mat4 inverted", "[mat4]")
{
    {
        const auto m = tinyla::mat4f{tinyla::mat_init::identity};
        compare(m.inverted(), m);
    }
    {
        const auto m = unique;
        compare(m.inverted(), tinyla::mat4f{tinyla::mat_init::identity});
    }
    {
        const auto m = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
        constexpr auto a = std::array<std::array<float, 4>, 4> {
             0.577350f,  0.000000f, -0.000000f,  0.000000f,
             0.000000f,  0.577350f,  0.000000f, -0.000000f,
            -0.000000f,  0.000000f, -0.000000f, -1.000000f,
             0.000000f, -0.000000f, -4.999500f,  5.000500f
        };
        compare(m.inverted(), a);
    }
}

TEST_CASE("mat4 * vec4", "[mat4]")
{
    const auto m = tinyla::mat4i {
         1,  2,  3,  4,
         5,  6,  7,  8,
         9, 10, 11, 12,
        13, 14, 15, 16
    };
    constexpr auto v = tinyla::vec4i {1, 2, 3, 4};
    const auto mv = m * v;
    compare(mv, tinyla::vec4i{30, 70, 110, 150});
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}

