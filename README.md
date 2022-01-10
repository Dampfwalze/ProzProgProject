# ProzProgProject

## Software Requirements
- [CMake](https://cmake.org/)

## Dependencies
- [SDL2](https://libsdl.org)
- [SDL_Image 2.0](https://www.libsdl.org/projects/SDL_image/)
- [SDL_ttf 2.0](https://www.libsdl.org/projects/SDL_ttf/)
- [FreeType](https://freetype.org/)

All dependencies are resolved automaticlaly, there is no need to install any dependency

## How to run
To configure the project into a new build directory, run:
```
cmake -S path/to/source -B path/to/build -G"MinGW Makefiles"
```
To build it, run:
```
cmake --build path/to/build
```
This will create an executable in the root of the build folder