cmake_minimum_required(VERSION 3.29)

project(tinyla)

set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 REQUIRED)

include_directories(include)

if (MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(tinyla_vec_tests test/vec_tests.cpp)
target_link_libraries(tinyla_vec_tests PRIVATE Catch2::Catch2)

add_executable(tinyla_mat_tests test/mat_tests.cpp)
target_link_libraries(tinyla_mat_tests PRIVATE Catch2::Catch2)

add_executable(tinyla_geom_tests test/geom_tests.cpp)
target_link_libraries(tinyla_geom_tests PRIVATE Catch2::Catch2)

add_executable(tinyla_mat_benchs "test/geom_benchs.cpp")
target_link_libraries(tinyla_mat_benchs PRIVATE Catch2::Catch2)

include(CTest)
include(Catch)
catch_discover_tests(tinyla_vec_tests)
catch_discover_tests(tinyla_mat_tests)
catch_discover_tests(tinyla_geom_tests)
