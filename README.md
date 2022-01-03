# ProzProgProject

# Software Requirements
- MinGW
- [CMake](https://cmake.org/)
- [SDL2](https://www.libsdl.org/) for MinGW
- [FreeType](https://freetype.org/) for MinGW

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