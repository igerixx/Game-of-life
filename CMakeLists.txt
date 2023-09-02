cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

if (WIN32)
    set(EXECUTABLE_SUFFIX ".exe")
endif()

add_executable(GameOfLife src/main.cpp)

target_link_libraries(GameOfLife PRIVATE sfml-system sfml-window sfml-graphics)
