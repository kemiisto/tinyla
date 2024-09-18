#include <tinyla/vec.h>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm.hpp>

constexpr auto glmVec4A = glm::vec4{1.0f, 2.0f, 3.0f, 4.0f};
constexpr auto glmVec4B = glm::vec4{4.0f, 5.0f, 6.0f, 7.0f};

constexpr auto tglVec4A = tinyla::Vec4{1.0f, 2.0f, 3.0f, 4.0f};
constexpr auto tglVec4B = tinyla::Vec4{4.0f, 5.0f, 6.0f, 7.0f};

template<std::size_t N, typename T>
void compare(const tinyla::Vec<N,T>& tglVec, const glm::vec<N,T>& glmVec)
{
    for (std::size_t i = 0; i < N; ++i) {
        CAPTURE(i);
        REQUIRE(tglVec[i] == Catch::Approx(glmVec[i]));
    }
}

TEST_CASE("Vec2 size", "[Vec2]")
{
    REQUIRE(sizeof(tinyla::Vec2) == 2 * sizeof(float));
}

TEST_CASE("Vec2 is constructed from initializer list", "[Vec2]")
{
    tinyla::Vec2 tglVec {0.0f, 0.1f};
    glm::vec2 glmVec {0.0f, 0.1f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec2 length", "[Vec2]")
{
    REQUIRE(tinyla::Vec2{ 0.0f,  0.0f}.length() == 0.0f);               // zero
    REQUIRE(tinyla::Vec2{ 1.0f,  0.0f}.length() == 1.0f);               // 1x
    REQUIRE(tinyla::Vec2{ 0.0f,  1.0f}.length() == 1.0f);               // 1y
    REQUIRE(tinyla::Vec2{-1.0f,  0.0f}.length() == 1.0f);               // -1x
    REQUIRE(tinyla::Vec2{ 0.0f, -1.0f}.length() == 1.0f);               // -1y
    REQUIRE(tinyla::Vec2{ 2.0f, -2.0f}.length() == std::sqrt(8.0f));    // two
}

TEST_CASE("Vec2 normalized", "[Vec2]")
{
    // For zero vector glm::normalize() returns vector of NaNs.
    // We rather simply return the zero vector back.
    compare(tinyla::Vec2{ 0.0f,  0.0f}.normalized(), glm::vec2{ 0.0f,  0.0f});                  // zero
    compare(tinyla::Vec2{ 1.0f,  0.0f}.normalized(), glm::normalize(glm::vec2{ 1.0f,  0.0f}));  // 1x
    compare(tinyla::Vec2{ 0.0f,  1.0f}.normalized(), glm::normalize(glm::vec2{ 0.0f,  1.0f}));  // 1y
    compare(tinyla::Vec2{-1.0f,  0.0f}.normalized(), glm::normalize(glm::vec2{-1.0f,  0.0f}));  // -1x
    compare(tinyla::Vec2{ 0.0f, -1.0f}.normalized(), glm::normalize(glm::vec2{ 0.0f, -1.0f}));  // -1y
    compare(tinyla::Vec2{ 2.0f, -2.0f}.normalized(), glm::normalize(glm::vec2{ 2.0f, -2.0f}));  // two
}

TEST_CASE("Vec2 normalize", "[Vec2]")
{
    // For zero vector glm::normalize() returns vector of NaNs.
    // We rather simply return the zero vector back.

    auto v0 = tinyla::Vec2{ 0.0f,  0.0f};
    auto v1 = tinyla::Vec2{ 1.0f,  0.0f};
    auto v2 = tinyla::Vec2{ 0.0f,  1.0f};
    auto v3 = tinyla::Vec2{-1.0f,  0.0f};
    auto v4 = tinyla::Vec2{ 0.0f, -1.0f};
    auto v5 = tinyla::Vec2{ 2.0f, -2.0f};

    v0.normalize();
    v1.normalize();
    v2.normalize();
    v3.normalize();
    v4.normalize();
    v5.normalize();

    compare(v0, glm::vec2{ 0.0f,  0.0f});                  // zero
    compare(v1, glm::normalize(glm::vec2{ 1.0f,  0.0f}));  // 1x
    compare(v2, glm::normalize(glm::vec2{ 0.0f,  1.0f}));  // 1y
    compare(v3, glm::normalize(glm::vec2{-1.0f,  0.0f}));  // -1x
    compare(v4, glm::normalize(glm::vec2{ 0.0f, -1.0f}));  // -1y
    compare(v5, glm::normalize(glm::vec2{ 2.0f, -2.0f}));  // two
}

TEST_CASE("Vec3 is constructed from initializer list", "[Vec3]")
{
    tinyla::Vec3 tglVec {0.0f, 0.1f, 0.2f};
    glm::vec3 glmVec {0.0f, 0.1f, 0.2f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec3 length", "[Vec3]")
{
    REQUIRE(tinyla::Vec3{ 0.0f,  0.0f,  0.0f}.length() == 0.0f);                // zero
    REQUIRE(tinyla::Vec3{ 1.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1x
    REQUIRE(tinyla::Vec3{ 0.0f,  1.0f,  0.0f}.length() == 1.0f);                // 1y
    REQUIRE(tinyla::Vec3{ 0.0f,  0.0f,  1.0f}.length() == 1.0f);                // 1z
    REQUIRE(tinyla::Vec3{-1.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1x
    REQUIRE(tinyla::Vec3{ 0.0f, -1.0f,  0.0f}.length() == 1.0f);                // -1y
    REQUIRE(tinyla::Vec3{ 0.0f,  0.0f, -1.0f}.length() == 1.0f);                // -1z
    REQUIRE(tinyla::Vec3{ 2.0f, -2.0f,  2.0f}.length() == std::sqrt(12.0f));    // two
}

TEST_CASE("Vec4 is constructed from initializer list", "[Vec4]")
{
    tinyla::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 glmVec {0.0f, 0.1f, 0.2f, 0.3f};
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 component access", "[Vec4]")
{
    tinyla::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    glm::vec4 glmVec {0.0f, 0.1f, 0.2f, 0.3f};
    REQUIRE(tglVec.x() == Catch::Approx(glmVec.x));
    REQUIRE(tglVec.y() == Catch::Approx(glmVec.y));
    REQUIRE(tglVec.z() == Catch::Approx(glmVec.z));
    REQUIRE(tglVec.w() == Catch::Approx(glmVec.w));
}

TEST_CASE("Vec4 data", "[Vec4]")
{
    tinyla::Vec4 tglVec {0.0f, 0.1f, 0.2f, 0.3f};
    float a[4] {0.0f, 0.1f, 0.2f, 0.3f};

    float* data = tglVec.data();
    for (std::size_t i = 0; i < 4; ++i) {
        REQUIRE(data[i] == Catch::Approx(a[i]));
    }
}

TEST_CASE("Vec4 operator+=", "[Vec4]")
{
    auto tglVec = tglVec4A;
    tglVec += tglVec4B;
    auto glmVec = glmVec4A + glmVec4B;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator+", "[Vec4]")
{
    auto tglVec = tglVec4A + tglVec4B;
    auto glmVec = glmVec4A + glmVec4B;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator-=", "[Vec4]")
{
    auto tglVec = tglVec4A;
    tglVec -= tglVec4B;
    auto glmVec = glmVec4A - glmVec4B;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator-", "[Vec4]")
{
    auto tglVec = tglVec4A - tglVec4B;
    auto glmVec = glmVec4A - glmVec4B;
    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 operator*=", "[Vec4]")
{
    {
        auto tglVec = tglVec4A;
        tglVec *= 2.0f;
        auto glmVec = glmVec4A * 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVec4A;
        tglVec *= tglVec4B;
        auto glmVec = glmVec4A * glmVec4B;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 operator*", "[Vec4]")
{
    {
        auto tglVec = 2.0f * tglVec4B;
        auto glmVec = 2.0f * glmVec4B;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVec4A * 2.0f;
        auto glmVec = glmVec4A * 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVec4A * tglVec4B;
        auto glmVec = glmVec4A * glmVec4B;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 operator/=", "[Vec4]")
{
    {
        auto tglVec = tglVec4A;
        tglVec /= 2.0f;
        auto glmVec = glmVec4A / 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVec4A;
        tglVec /= tglVec4B;
        auto glmVec = glmVec4A / glmVec4B;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 operator/", "[Vec4]")
{
    {
        auto tglVec = tglVec4A / 2.0f;
        auto glmVec = glmVec4A / 2.0f;
        compare(tglVec, glmVec);
    }

    {
        auto tglVec = tglVec4A / tglVec4B;
        auto glmVec = glmVec4A / glmVec4B;
        compare(tglVec, glmVec);
    }
}

TEST_CASE("Vec4 unary operator-", "[Vec4]")
{
    tinyla::Vec4 tglVec {0.0f, -0.1f, 0.2f, -0.3f};
    tglVec = -tglVec;

    glm::vec4 glmVec {0.0f, -0.1f, 0.2f, -0.3f};
    glmVec = -glmVec;

    compare(tglVec, glmVec);
}

TEST_CASE("Vec4 length", "[Vec4]")
{
    REQUIRE(tinyla::Vec4{ 0.0f,  0.0f,  0.0f,  0.0f}.length() == 0.0f);                // zero
    REQUIRE(tinyla::Vec4{ 1.0f,  0.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1x
    REQUIRE(tinyla::Vec4{ 0.0f,  1.0f,  0.0f,  0.0f}.length() == 1.0f);                // 1y
    REQUIRE(tinyla::Vec4{ 0.0f,  0.0f,  1.0f,  0.0f}.length() == 1.0f);                // 1z
    REQUIRE(tinyla::Vec4{ 0.0f,  0.0f,  0.0f,  1.0f}.length() == 1.0f);                // 1w
    REQUIRE(tinyla::Vec4{-1.0f,  0.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1x
    REQUIRE(tinyla::Vec4{ 0.0f, -1.0f,  0.0f,  0.0f}.length() == 1.0f);                // -1y
    REQUIRE(tinyla::Vec4{ 0.0f,  0.0f, -1.0f,  0.0f}.length() == 1.0f);                // -1z
    REQUIRE(tinyla::Vec4{ 0.0f,  0.0f,  0.0f, -1.0f}.length() == 1.0f);                // -1w
    REQUIRE(tinyla::Vec4{ 2.0f, -2.0f,  2.0f,  2.0f}.length() == std::sqrt(16.0f));    // two
}

TEST_CASE("Vecn")
{
    auto v = tinyla::Vec{0, 1, 2, 3, 4, 5};
    REQUIRE(v[0] == 0);
    REQUIRE(v[1] == 1);
    REQUIRE(v[2] == 2);
    REQUIRE(v[3] == 3);
    REQUIRE(v[4] == 4);
    REQUIRE(v[5] == 5);
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}
