#include <tinyla/geom.hpp>
#include <tinyla/mat.hpp>
#include <tinyla/util.hpp>
#define CATCH_CONFIG_RUNNER
#include <catch2/catch_all.hpp>

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

TEST_CASE("mat4 scaling benchmark", "[mat4]")
{
    BENCHMARK("pre-scale by pre_scale") {
        auto m = unique;
        tinyla::geom::pre_scale(m, tinyla::vec3f{2.0f, 3.0f, 4.0f});
        return m;
    };

    BENCHMARK("pre-scale by matrix multiplication") {
        auto t = tinyla::geom::scaling(tinyla::vec3f{2.0f, 3.0f, 4.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

TEST_CASE("mat4 translation benchmark", "[mat4]")
{
    BENCHMARK("pre-translate by pre_translate") {
        auto m = unique;
        tinyla::geom::pre_translate(m, tinyla::vec3f{1.0f, 2.0f, 3.0f});
        return m;
    };

    BENCHMARK("pre-translate by matrix multiplication") {
        auto t = tinyla::geom::translation(tinyla::vec3f{1.0f, 2.0f, 3.0f});
        auto m = unique;
        m = t * m;
        return m;
    };
}

int main(int argc, const char* argv[])
{
    return Catch::Session().run(argc, argv);
}

