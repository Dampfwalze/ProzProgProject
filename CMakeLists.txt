cmake_minimum_required(VERSION 3.14)

project(ProzProgProject VERSION 0.0.0)

include(FetchContent)

set(C_STANDARD 17)

if(NOT WIN32)
    set( CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -Wall -fPIC" )
    set( CMAKE_C_FLAGS  "${CMAKE_C_FLAGS} -Wall -fPIC" )
endif()

# set output directory for all executables and dynamic libraries
set( OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_DIRECTORY}")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY         "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG   "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY         "${OUTPUT_DIRECTORY}")
#set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${OUTPUT_DIRECTORY}")
#set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${OUTPUT_DIRECTORY}")
#set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")

set(PNG_ARM_NEON "off")
add_compile_definitions(PNG_ARM_NEON_OPT=0)

# fetch SDL2
FetchContent_Declare(
    lib_sdl2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.0.18
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps-external/lib_sdl2-src
)

# fetch SDL_image
FetchContent_Declare(
    lib_sdl_image
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_image.git
    GIT_TAG main
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/deps-external/lib_sdl_image-src
)

FetchContent_MakeAvailable(lib_sdl2 lib_sdl_image)

# Find project sources
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
# Find project headers
file(GLOB_RECURSE HEADRES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")

# Include SDL2_TTF from within the project
add_subdirectory(deps/SDL2_ttf)

add_executable(app ${SOURCES} ${HEADRES} )

target_include_directories(
    app PUBLIC
    ${PROJECT_SOURCE_DIR}/src/include
)

# link libraries
if(WIN32)
    target_link_libraries(app PRIVATE -lmingw32 SDL2::SDL2main SDL2::SDL2 SDL2_image SDL2_TTF)
else()
    target_link_libraries(app PRIVATE SDL2::SDL2 SDL2_image SDL2_TTF -lm)
endif()
