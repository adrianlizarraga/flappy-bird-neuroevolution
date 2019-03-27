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
    flappy.cpp
)

target_link_libraries(flappy
    neuralnet
    sfml-graphics
)
