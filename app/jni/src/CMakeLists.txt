cmake_minimum_required(VERSION 3.6)

project(BOULDROVKA)

find_library(SDL2 SDL2_image SDL2_ttf)

add_library(main SHARED)

target_sources(main PRIVATE main.cpp)

target_link_libraries(main SDL2 SDL2_image SDL2_ttf log)


