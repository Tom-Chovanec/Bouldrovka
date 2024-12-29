cmake_minimum_required(VERSION 3.10)

project(Bouldrovka)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

target_compile_features(cxx_std_23)

# SDL sources are in a subfolder named "SDL"
add_subdirectory(SDL)
add_subdirectory(SDL_image)

#add_subdirectory(SDL_mixer)
#add_subdirectory(SDL_ttf)

# Your game and its CMakeLists.txt are in a subfolder named "src"
add_subdirectory(src)

