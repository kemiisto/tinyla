cmake_minimum_required(VERSION 4.0)

project(tinyla)

set(CMAKE_CXX_STANDARD 23)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG        v3.8.1
)
FetchContent_MakeAvailable(Catch2)
link_libraries(Catch2::Catch2WithMain)

include_directories(include)

if (MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

enable_testing()
include(CTest)
include(Catch)

add_executable(tinyla_vec_tests test/vec_tests.cpp)
catch_discover_tests(tinyla_vec_tests)

add_executable(tinyla_mat_tests test/mat_tests.cpp)
catch_discover_tests(tinyla_mat_tests)

add_executable(tinyla_geom_tests test/geom_tests.cpp)
catch_discover_tests(tinyla_geom_tests)

add_executable(tinyla_mat_benchs test/geom_benchs.cpp)
