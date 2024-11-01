cmake_minimum_required(VERSION 3.5)

project(tinyla)

set(CMAKE_CXX_STANDARD 23)

find_package(Catch2 3 REQUIRED)
find_package(glm CONFIG REQUIRED)

include_directories(include)

if (MSVC)
    add_compile_options(/W3 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()

add_executable(tinyla_mat_tests test/mat_tests.cpp)
target_link_libraries(tinyla_mat_tests PRIVATE Catch2::Catch2 glm::glm)

add_executable(tinyla_vec_tests test/vec_tests.cpp)
target_link_libraries(tinyla_vec_tests PRIVATE Catch2::Catch2 glm::glm)
