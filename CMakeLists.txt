cmake_minimum_required(VERSION 3.14)

project(ProzProgProject VERSION 0.0.0)

# Find SDL2 installed on the system, sets SDL2_INCLUDE_DIRS to SDLs include directory and SDL2_LIBRARIES to all linking files
find_package(SDL2 REQUIRED)
message("SDL include path: ${SDL2_INCLUDE_DIRS}")
message("SDL version: ${SDL2_LIBRARIES}")

# Find project source
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")

add_executable(app ${SOURCES})

# Link SDL2 into the project
target_link_libraries( app PRIVATE -lmingw32 ${SDL2_LIBRARIES} )
target_include_directories(
    app PUBLIC
    ${SDL2_INCLUDE_DIRS}
)

file(COPY ${PROJECT_SOURCE_DIR}/SDL2.dll DESTINATION ${PROJECT_BINARY_DIR}) 