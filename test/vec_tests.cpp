#include <tinyla/vec.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#include "compare.hpp"

constexpr auto v4a = tinyla::vec4f{1.0f, 2.0f, 3.0f, 4.0f};
constexpr auto v4b = tinyla::vec4f{4.0f, 5.0f, 6.0f, 7.0f};

constexpr auto v4a_plus_v4b = tinyla::vec4f{1.0f + 4.0f, 2.0f + 5.0f, 3.0f + 6.0f, 4.0f + 7.0f};
constexpr auto v4a_minus_v4b = tinyla::vec4f{1.0f - 4.0f, 2.0f - 5.0f, 3.0f - 6.0f, 4.0f - 7.0f};
constexpr auto v4a_times_v4b = tinyla::vec4f{1.0f * 4.0f, 2.0f * 5.0f, 3.0f * 6.0f, 4.0f * 7.0f};
constexpr auto v4a_divided_by_v4b = tinyla::vec4f{1.0f / 4.0f, 2.0f / 5.0f, 3.0f / 6.0f, 4.0f / 7.0f};

TEST_CASE("vec2 size", "[vec2]")
{
    REQUIRE(sizeof(tinyla::vec2f) == 2 * sizeof(float));
}

TEST_CASE("vec2 is constructed from initializer list", "[vec2]")
{
    constexpr auto v = tinyla::vec2f{0.0f, 0.1f};
    constexpr auto a = std::array{0.0f, 0.1f};
    compare(v, a);
}

TEST_CASE("vec2 length", "[vec2]")
{
    REQUIRE(tinyla::vec2f{ 0.0f,  0.0f}.length() == 0.0f);               // zero
    REQUIRE(tinyla::vec2f{ 1.0f,  0.0f}.length() == 1.0f);               // 1x
    REQUIRE(tinyla::vec2f{ 0.0f,  1.0f}.length() == 1.0f);               // 1y
    REQUIRE(tinyla::vec2f{-1.0f,  0.0f}.length() == 1.0f);               // -1x
    REQUIRE(tinyla::vec2f{ 0.0f, -1.0f}.length() == 1.0f);               // -1y
    REQUIRE(tinyla::vec2f{ 2.0f, -2.0f}.length() == std::sqrt(8.0f));    // two
}

TEST_CASE("vec2 normalized", "[vec2]")
{
    // For zero vector glm::normalize() returns vector of NaNs.
    // We rather simply return the zero vector back.
    compare(tinyla::vec2f{ 0.0f,  0.0f}.normalized(), std::array{ 0.000000f,  0.000000f});  // zero
    compare(tinyla::vec2f{ 1.0f,  0.0f}.normalized(), std::array{ 1.000000f,  0.000000f});  // 1x
    compare(tinyla::vec2f{ 0.0f,  1.0f}.normalized(), std::array{ 0.000000f,  1.000000f});  // 1y
    compare(tinyla::vec2f{-1.0f,  0.0f}.normalized(), std::array{-1.000000f,  0.000000f});  // -1x
    compare(tinyla::vec2f{ 0.0f, -1.0f}.normalized(), std::array{ 0.000000f, -1.000000f});  // -1y
    compare(tinyla::vec2f{ 2.0f, -2.0f}.normalized(), std::array{ 0.707107f, -0.707107f});  // two
}

TEST_CASE("vec2 normalize", "[vec2]")
{
    // For zero vector glm::normalize() returns vector of NaNs.
    // We rather simply return the zero vector back.

    auto v0 = tinyla::vec2f{ 0.0f,  0.0f};
    auto v1 = tinyla::vec2f{ 1.0f,  0.0f};
    auto v2 = tinyla::vec2f{ 0.0f,  1.0f};
    auto v3 = tinyla::vec2f{-1.0f,  0.0f};
    auto v4 = tinyla::vec2f{ 0.0f, -1.0f};
    auto v5 = tinyla::vec2f{ 2.0f, -2.0f};

    v0.normalize();
    v1.normalize();
    v2.normalize();
    v3.normalize();
    v4.normalize();
    v5.normalize();

    compare(v0, std::array{ 0.000000f,  0.000000f});  // zero
    compare(v1, std::array{ 1.000000f,  0.000000f});  // 1x
    compare(v2, std::array{ 0.000000f,  1.000000f});  // 1y
    compare(v3, std::array{-1.000000f,  0.000000f});  // -1x
    compare(v4, std::array{ 0.000000f, -1.000000f});  // -1y
    compare(v5, std::array{ 0.707107f, -0.707107f});  // two
}

TEST_CASE("vec3 is constructed from initializer list", "[vec3]")
{
    constexpr auto v = tinyla::vec3f{0.0f, 0.1f, 0.2f};
    constexpr auto a = std::array{0.0f, 0.1f, 0.2f};
    compare(v, a);
}

TEST_CASE("vec3 length", "[vec3]")
{
    REQUIRE(tinyla::vec3f{ 0.0f,  0.0f,  0.0f}.length() == 0.0f);                // zero
    REQUIRE(tinyla::vec3f{ 1.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1x
    REQUIRE(tinyla::vec3f{ 0.0f,  1.0f,  0.0f}.length() == 1.0f);                // 1y
    REQUIRE(tinyla::vec3f{ 0.0f,  0.0f,  1.0f}.length() == 1.0f);                // 1z
    REQUIRE(tinyla::vec3f{-1.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1x
    REQUIRE(tinyla::vec3f{ 0.0f, -1.0f,  0.0f}.length() == 1.0f);                // -1y
    REQUIRE(tinyla::vec3f{ 0.0f,  0.0f, -1.0f}.length() == 1.0f);                // -1z
    REQUIRE(tinyla::vec3f{ 2.0f, -2.0f,  2.0f}.length() == std::sqrt(12.0f));    // two
}

TEST_CASE("vec4 is constructed from initializer list", "[vec4]")
{
    constexpr auto m = tinyla::vec4f{0.0f, 0.1f, 0.2f, 0.3f};
    constexpr auto a = std::array{0.0f, 0.1f, 0.2f, 0.3f};
    compare(m, a);
}

TEST_CASE("vec4 component access", "[vec4]")
{
    constexpr auto m = tinyla::vec4f{0.0f, 0.1f, 0.2f, 0.3f};
    constexpr auto a = std::array{0.0f, 0.1f, 0.2f, 0.3f};
    REQUIRE(m.x() == Catch::Approx(a[0]));
    REQUIRE(m.y() == Catch::Approx(a[1]));
    REQUIRE(m.z() == Catch::Approx(a[2]));
    REQUIRE(m.w() == Catch::Approx(a[3]));
}

TEST_CASE("vec4 data", "[vec4]")
{
    tinyla::vec4f tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    float a[4] {0.0f, 0.1f, 0.2f, 0.3f};

    float* data = tglVec.data();
    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(data[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("vec4 operator+=", "[vec4]")
{
    auto v = v4a;
    v += v4b;
    compare(v, v4a_plus_v4b);
}

TEST_CASE("vec4 operator+", "[vec4]")
{
    constexpr auto v = v4a + v4b;
    compare(v, v4a_plus_v4b);
}

TEST_CASE("vec4 operator-=", "[vec4]")
{
    auto v = v4a;
    v -= v4b;
    compare(v, v4a_minus_v4b);
}

TEST_CASE("vec4 operator-", "[vec4]")
{
    constexpr auto tglVec = v4a - v4b;
    compare(tglVec, v4a_minus_v4b);
}

TEST_CASE("vec4 operator*=", "[vec4]")
{
    auto v = v4a;
    v *= v4b;
    compare(v, v4a_times_v4b);
}

TEST_CASE("vec4 operator*", "[vec4]")
{
    constexpr auto v = v4a * v4b;
    compare(v, v4a_times_v4b);
}

TEST_CASE("vec4 operator/=", "[vec4]")
{
    auto v = v4a;
    v /= v4b;
    compare(v, v4a_divided_by_v4b);
}

TEST_CASE("vec4 operator/", "[vec4]")
{
    constexpr auto v = v4a / v4b;
    compare(v, v4a_divided_by_v4b);
}

TEST_CASE("vec4 unary operator-", "[vec4]")
{
    auto v = tinyla::vec4f{0.0f, -0.1f, 0.2f, -0.3f};
    v = -v;
    compare(v, tinyla::vec4f{0.0f, 0.1f, -0.2f, 0.3f});
}

TEST_CASE("vec4 length", "[vec4]")
{
    REQUIRE(tinyla::vec4f{ 0.0f,  0.0f,  0.0f,  0.0f}.length() == 0.0f);                // zero
    REQUIRE(tinyla::vec4f{ 1.0f,  0.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1x
    REQUIRE(tinyla::vec4f{ 0.0f,  1.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1y
    REQUIRE(tinyla::vec4f{ 0.0f,  0.0f,  1.0f,  0.0f}.length() == 1.0f);                // 1z
    REQUIRE(tinyla::vec4f{ 0.0f,  0.0f,  0.0f,  1.0f}.length() == 1.0f);                // 1w
    REQUIRE(tinyla::vec4f{-1.0f,  0.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1x
    REQUIRE(tinyla::vec4f{ 0.0f, -1.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1y
    REQUIRE(tinyla::vec4f{ 0.0f,  0.0f, -1.0f,  0.0f}.length() == 1.0f);                // -1z
    REQUIRE(tinyla::vec4f{ 0.0f,  0.0f,  0.0f, -1.0f}.length() == 1.0f);                // -1w
    REQUIRE(tinyla::vec4f{ 2.0f, -2.0f,  2.0f,  2.0f}.length() == std::sqrt(16.0f));    // two
}

TEST_CASE("vecn")
{
    auto v = tinyla::vec{0, 1, 2, 3, 4, 5};
    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 3);
    REQUIRE(v[4] == 4);
    REQUIRE(v[5] == 5);
}

TEST_CASE("vec4 from vec3")
{
    auto v3 = tinyla::vec3i{0, 1, 2};
    auto v4 = tinyla::vec4i{v3, 3};
    REQUIRE(v4[0] == 0);
    REQUIRE(v4[1] == 1);
    REQUIRE(v4[2] == 2);
    REQUIRE(v4[3] == 3);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
