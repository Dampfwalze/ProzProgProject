cmake_minimum_required(VERSION 3.14)

project(ProzProgProject VERSION 0.0.0)

# Find SDL2 installed on the system, sets SDL2_INCLUDE_DIRS to SDLs include directory and SDL2_LIBRARIES to all linking files
find_package(SDL2 REQUIRED)
message("SDL include path: ${SDL2_INCLUDE_DIRS}")
message("SDL library: ${SDL2_LIBRARIES}")

# Find project sources
file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
# Find project headers
file(GLOB_RECURSE HEADRES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.h")

# Include SDL2_TTF from within the project
add_subdirectory(deps/SDL2_ttf)
add_subdirectory(deps/SDL2_Image)

add_executable(app ${SOURCES} ${HEADRES} )

# Link SDL2 into the project
target_link_libraries( app PRIVATE -lmingw32 ${SDL2_LIBRARIES} SDL2_TTF SDL2_Image)
target_include_directories(
    app PUBLIC
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}/SDL2
    ${PROJECT_SOURCE_DIR}/deps/SDL2_TTF/include
    ${PROJECT_SOURCE_DIR}/deps/SDL2_Image/include
    ${PROJECT_SOURCE_DIR}/src/include
)


file(COPY ${PROJECT_SOURCE_DIR}/libfreetype-6.dll DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/SDL2.dll DESTINATION ${PROJECT_BINARY_DIR})
#file(COPY ${PROJECT_SOURCE_DIR}/SDL2_ttf.dll DESTINATION ${PROJECT_BINARY_DIR})
file(COPY ${PROJECT_SOURCE_DIR}/zlib1.dll DESTINATION ${PROJECT_BINARY_DIR})