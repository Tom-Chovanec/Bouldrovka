cmake_minimum_required(VERSION 3.6)

project(Bouldrovka)


add_subdirectory(SDL)

add_subdirectory(SDL_image)
add_subdirectory(SDL_ttf)

#add_subdirectory(SDL_mixer)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(src)

