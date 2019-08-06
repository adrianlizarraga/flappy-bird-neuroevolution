cmake_minimum_required(VERSION 3.5)

project(flappy-bird-neuroevolution)

############################################################
# Build local neuralnet library
############################################################
add_subdirectory(neuralnet)

############################################################
# Find SFML
############################################################
find_package(SFML 2.5 COMPONENTS graphics REQUIRED)

############################################################
# Create excutable and link libraries
############################################################
add_executable(flappy
    src/flappy.cpp
    src/Renderable.cpp
    src/Bird.cpp
    src/Ground.cpp
    src/Background.cpp
    src/Pipe.cpp
    src/AssetManager.cpp
    src/Game.cpp
    src/Menu.cpp
)

target_include_directories(flappy
    PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(flappy
    neuralnet
    sfml-graphics
)

# Copy assets to build directory
file(COPY data DESTINATION ${CMAKE_BINARY_DIR})
