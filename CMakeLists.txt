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
    src/entity/Renderable.cpp
    src/entity/Bird.cpp
    src/entity/Ground.cpp
    src/entity/Background.cpp
    src/entity/Pipe.cpp
    src/asset/AssetManager.cpp
    src/Game.cpp
    src/Menu.cpp
    src/states/TitleScreenState.cpp
    src/states/FadeState.cpp
    src/states/StateStack.cpp
    src/states/PlayState.cpp
    src/states/CPUPlayState.cpp
    src/states/TrainState.cpp
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
