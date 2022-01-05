cmake_minimum_required(VERSION 3.14)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(SDL2_Image_INCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(SDL2 REQUIRED)

file(GLOB SDL2_IMAGE_SOURCES "*.c")
file(GLOB SDL2_IMAGE_INCLUDES "include/SDL2/*.h")

add_library(SDL2_Image ${SDL2_IMAGE_SOURCES} ${SDL2_IMAGE_INCLUDES})

target_link_libraries(SDL2_Image SDL2::SDL2)

target_include_directories(SDL2_Image PUBLIC ${SDL2_Image_INCLUDE}/SDL2)