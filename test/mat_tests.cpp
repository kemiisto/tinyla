#include <tinyla/mat.h>
#include <tinyla/util.h>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>
#define GLM_FORCE_SIZE_T_LENGTH
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>

const auto zero = tinyla::mat4f {
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f
};

const auto unique = tinyla::mat4f {
    1.0f, 2.0f, 3.0f, 4.0f,
    5.0f, 6.0f, 7.0f, 8.0f,
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
void compare(const tinyla::mat<N,T>& tglMatrix, const glm::mat<N,N,T>& glmMatrix)
{
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < N; ++j) {
            CAPTURE(i);
            CAPTURE(j);
            REQUIRE(tglMatrix(i, j) == Catch::Approx(glmMatrix[i][j]));
        }
    }
}

TEST_CASE("Mat3 is constructed from initializer list", "[Mat3]")
{
    tinyla::mat3f tglMatrix {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };

    float a[] = {
        0.0f, 0.1f, 0.2f,
        1.0f, 1.1f, 1.2f,
        2.0f, 2.1f, 2.2f
    };
    glm::mat3 glmMatrix = glm::transpose(glm::make_mat3(a));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed from initializer list", "[Mat4]")
{
    tinyla::mat4f tglMatrix {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };

    float a[] = {
        0.0f, 0.1f, 0.2f, 0.3f,
        1.0f, 1.1f, 1.2f, 1.3f,
        2.0f, 2.1f, 2.2f, 2.3f,
        3.0f, 3.1f, 3.2f, 3.3f
    };
    glm::mat4 glmMatrix = glm::transpose(glm::make_mat4(a));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 is constructed as identity matrix", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    auto glmMatrix = glm::mat4(1.0f);
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 scaling", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::scale(glmMatrix, glm::vec3(2.0f, 3.0f, 4.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preScale", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.pre_scale(tinyla::vec3f{2.0f, 3.0f, 4.0f});

    auto scalingMatrix = glm::mat4(1.0f);
    scalingMatrix = glm::scale(scalingMatrix, glm::vec3(2.0f, 3.0f, 4.0f));
    auto glmMatrix = scalingMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postScale", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.post_scale(tinyla::vec3f{2.0f, 3.0f, 4.0f});

    auto glmMatrix = glm::make_mat4(unique.data());
    glmMatrix = glm::scale(glmMatrix, glm::vec3(2.0f, 3.0f, 4.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 scaling benchmark", "[Mat4]")
{
    BENCHMARK("preScale") {
        auto m = unique;
        m.pre_scale(tinyla::vec3f{2.0f, 3.0f, 4.0f});
        return m;
    };

    BENCHMARK("preScale by matrix multiplication") {
        auto t = tinyla::mat4f::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

TEST_CASE("Mat4 translation", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preTranslate", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.pre_translate(tinyla::vec3f{1.0f, 2.0f, 3.0f});

    auto translationMatrix = glm::mat4(1.0f);
    translationMatrix = glm::translate(translationMatrix, glm::vec3(1.0f, 2.0f, 3.0f));
    auto glmMatrix = translationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 translation benchmark", "[Mat4]")
{
    BENCHMARK("preTranslate") {
        auto m = unique;
        m.pre_translate(tinyla::vec3f{1.0f, 2.0f, 3.0f});
        return m;
    };

    BENCHMARK("preTranslate by matrix multiplication") {
        auto t = tinyla::mat4f::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

TEST_CASE("Mat4 postTranslate", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    tglMatrix.post_translate(tinyla::vec3f{1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around x-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.pre_rotate(45.0f, {1.0f, 0.0f, 0.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around x-axis", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    tglMatrix.post_rotate(45.0f, tinyla::vec3f{1.0f, 0.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around y-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.pre_rotate(45.0f, {0.0f, 1.0f, 0.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around y-axis", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    tglMatrix.post_rotate(45.0f, {0.0f, 1.0f, 0.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 preRotate around z-axis", "[Mat4]")
{
    auto tglMatrix = unique;
    tglMatrix.pre_rotate(45.0f, {0.0f, 0.0f, 1.0f});

    auto rotationMatrix = glm::mat4(1.0f);
    rotationMatrix = glm::rotate(rotationMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    auto glmMatrix = rotationMatrix * glm::make_mat4(unique.data());

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 postRotate around z-axis", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    tglMatrix.post_rotate(45.0f, {0.0f, 0.0f, 1.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::rotate(glmMatrix, tinyla::deg_to_rad(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));

    compare(tglMatrix, glmMatrix);
}

TEST_CASE("operator*=", "[Mat4]")
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

TEST_CASE("operator*", "[Mat4]")
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

TEST_CASE("Mat4 data", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
    tglMatrix.post_translate({1.0f, 2.0f, 3.0f});

    auto glmMatrix = glm::mat4(1.0f);
    glmMatrix = glm::translate(glmMatrix, glm::vec3(1.0f, 2.0f, 3.0f));

    auto* tglData = tglMatrix.data();
    auto* glmData = glm::value_ptr(glmMatrix);

    for (std::size_t i = 0; i < 4*4; ++i) {
        REQUIRE(tglData[i] == Catch::Approx(glmData[i]));
    }
}

TEST_CASE("Mat4 perspective", "[Mat4]")
{
    auto tglMatrix = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
    auto glmMatrix = glm::perspective(tinyla::deg_to_rad(60.0f), 1.0f, 0.1f, 1000.0f);
    compare(tglMatrix, glmMatrix);
}

TEST_CASE("Mat4 determinant", "[Mat4]")
{
    {
        auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
        auto glmMatrix = glm::mat4(1.0f);
        REQUIRE(tglMatrix.determinant() == Catch::Approx(glm::determinant(glmMatrix)));
    }
    {
        auto tglMatrix = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
        auto glmMatrix = glm::perspective(tinyla::deg_to_rad(60.0f), 1.0f, 0.1f, 1000.0f);
        REQUIRE(tglMatrix.determinant() == Catch::Approx(glm::determinant(glmMatrix)));
    }
}

TEST_CASE("Mat4 inverted", "[Mat4]")
{
    {
        auto tglMatrix = tinyla::mat4f(tinyla::mat_init::identity);
        auto glmMatrix = glm::mat4(1.0f);
        compare(tglMatrix.inverted(), glm::inverse(glmMatrix));
    }
    {
        auto tglMatrix = tinyla::mat4f::perspective(60.0f, 1.0f, 0.1f, 1000.0f);
        auto glmMatrix = glm::perspective(tinyla::deg_to_rad(60.0f), 1.0f, 0.1f, 1000.0f);
        tglMatrix = tglMatrix.inverted();
        glmMatrix = glm::inverse(glmMatrix);
        compare(tglMatrix.inverted(), glm::inverse(glmMatrix));
    }
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}

