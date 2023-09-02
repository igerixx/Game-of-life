cmake_minimum_required(VERSION 3.1)
project(GameOfLifeSFML)

find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

add_executable(GameOfLife main.cpp)

target_link_libraries(GameOfLife sfml-graphics)