cmake_minimum_required(VERSION 3.14)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(SDL2_TTF_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)
find_package(Freetype REQUIRED)

add_library(SDL2_TTF SDL_ttf.c)

target_link_libraries(SDL2_TTF SDL2::SDL2 Freetype::Freetype)

target_include_directories(SDL2_TTF PUBLIC ${FREETYPE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE}/SDL2)