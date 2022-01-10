cmake_minimum_required(VERSION 3.14)

include(FetchContent)

# fetch FreeType
FetchContent_Declare(
    lib_freetype
    GIT_REPOSITORY https://github.com/freetype/freetype.git
    GIT_TAG VER-2-11-1
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps-external/lib_freetype-src
)

FetchContent_MakeAvailable(lib_freetype)

set(SDL2_TTF_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(NOT (TARGET SDL2 OR TARGET SDL2-static))
    message("SDL2 could not be found")
    find_package(SDL2 REQUIRED)
endif()

add_library(SDL2_TTF STATIC SDL_ttf.c)

target_link_libraries(SDL2_TTF SDL2::SDL2 freetype)

target_include_directories(
    SDL2_TTF PUBLIC 
    ${SDL2_TTF_INCLUDE}/SDL2
    ${SDL2_TTF_INCLUDE}
)