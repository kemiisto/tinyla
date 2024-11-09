#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include "compare.hpp"
#include "data.hpp"
#include <array>

using namespace tinyla::literals;

TEST_CASE("mat3 is constructed from initializer list", "[mat3]")
{
    constexpr auto m = tinyla::mat3f {
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
    constexpr auto m = tinyla::mat4f {
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
    constexpr auto m = tinyla::mat4f{tinyla::mat_init::identity};
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
    constexpr auto m = tinyla::mat4f {
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
        constexpr auto m = tinyla::mat4f{tinyla::mat_init::identity};
        REQUIRE(m.determinant() == Catch::Approx(1.0f));
    }
    {
        constexpr auto m = unique;
        REQUIRE(m.determinant() == Catch::Approx(0.0f));
    }
}

TEST_CASE("mat4 inverted", "[mat4]")
{
    {
        constexpr auto m = tinyla::mat4f{tinyla::mat_init::identity};
        compare(m.inverted(), m);
    }
    {
        constexpr auto m = unique;
        compare(m.inverted(), tinyla::mat4f{tinyla::mat_init::identity});
    }
}

TEST_CASE("mat4 * vec4", "[mat4]")
{
    constexpr auto m = tinyla::mat4i {
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
