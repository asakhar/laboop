cmake_minimum_required(VERSION 3.12)

project(laboop LANGUAGES C)

add_library(set src/set.c)
add_library(sort_n_unif src/sort_and_unify.c)
add_library(input_lib src/input.c)

add_executable(main src/main.c)
add_executable(tests src/tests.c)

target_link_libraries(tests PUBLIC set sort_n_unif)

target_link_libraries(main PUBLIC set input_lib)