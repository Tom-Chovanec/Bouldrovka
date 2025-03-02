cmake_minimum_required(VERSION 3.6)

project(bouldrovka)

if(NOT TARGET SDL3::SDL3)
    find_package(SDL3 CONFIG)
endif()

# TTF
if(NOT TARGET SDL3::SDL3)
    find_library(SDL3_LIBRARY NAMES "SDL3")
    find_path(SDL3_INCLUDE_DIR NAMES "SDL3/SDL.h")
    add_library(SDL3::SDL3 UNKNOWN IMPORTED)
    set_property(TARGET SDL3::SDL3 PROPERTY IMPORTED_LOCATION "${SDL3_LIBRARY}")
    set_property(TARGET SDL3::SDL3 PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3::SDL3)
    message(FATAL_ERROR "Cannot find SDL3.

Possible ways to fix this:
- Use a SDL3 Android aar archive, and configure gradle to use it: prefab is required.
- Add add_subdirectory(path/to/SDL) to your CMake script, and make sure a vendored SDL is present there.
")
endif()

# TTF
if(NOT TARGET SDL3_ttf::SDL3_ttf)
    find_package(SDL3_ttf CONFIG)
endif()

if(NOT TARGET SDL3_ttf::SDL3_ttf)
    find_library(SDL3_TTF_LIBRARY NAMES "SDL3_ttf")
    find_path(SDL3_TTF_INCLUDE_DIR NAMES "SDL3_ttf/SDL_ttf.h")
    add_library(SDL3_ttf::SDL3_ttf UNKNOWN IMPORTED)
    set_property(TARGET SDL3_ttf::SDL3_ttf PROPERTY IMPORTED_LOCATION "${SDL3_TTF_LIBRARY}")
    set_property(TARGET SDL3_ttf::SDL3_ttf PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_TTF_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3_ttf::SDL3_ttf)
    message(FATAL_ERROR "Cannot find SDL3_ttf.")
endif()

# IMAGE
if(NOT TARGET SDL3_image::SDL3_image)
    find_package(SDL3_image CONFIG)
endif()

if(NOT TARGET SDL3_image::SDL3_image)
    find_library(SDL3_IMAGE_LIBRARY NAMES "SDL3_image")
    find_path(SDL3_IMAGE_INCLUDE_DIR NAMES "SDL3_image/SDL_image.h")
    add_library(SDL3_image::SDL3_image UNKNOWN IMPORTED)
    set_property(TARGET SDL3_image::SDL3_image PROPERTY IMPORTED_LOCATION "${SDL3_IMAGE_LIBRARY}")
    set_property(TARGET SDL3_image::SDL3_image PROPERTY INTERFACE_INCLUDE_DIRECTORIES "${SDL3_IMAGE_INCLUDE_DIR}")
endif()

if(NOT TARGET SDL3_image::SDL3_image)
    message(FATAL_ERROR "Cannot find SDL3_image.")
endif()

add_library(main SHARED
    app.cpp
    main.cpp
    boulder.cpp
    boulder_selection.cpp
    colors.cpp
    hold.cpp
    hold_selection.cpp
    icon.cpp
    image.cpp
    object.cpp
    renderer.cpp
    resource_manager.cpp
    scene.cpp
    simple_button.cpp
    text.cpp
)

target_link_libraries(main PRIVATE SDL3::SDL3)
target_link_libraries(main PRIVATE SDL3_ttf::SDL3_ttf)
target_link_libraries(main PRIVATE SDL3_image::SDL3_image)
