cmake_minimum_required(VERSION 3.10)
project(GameOfLife)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

cmake_minimum_required(VERSION 3.1)
project(GameOfLifeSFML)

add_executable(GameOfLife src/main.cpp)

target_link_libraries(GameOfLife sfml-graphics)

find_package(SFML 2.5 COMPONENTS system window graphics REQUIRED)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

if(APPLE OR UNIX)
    SET(PLATFORM_SPECIFIC_LIBS "-lpthread")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

    include_directories(/usr/local/include)
else()
    set(SFML_DIR "C:/Libraries/SFML/lib/cmake/SFML")
    set(SFML_STATIC_LIBRARIES TRUE)
endif()

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

if (SFML_FOUND)
    include_directories(${SFML_INCLUDE_DIR})
    target_link_libraries(GameOfLife sfml-graphics sfml-window sfml-system)
endif()
