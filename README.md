# ProzProgProject

# Software Requirements
- MinGW
- CMake
- SDL2 installed for MinGW on the system

## How to run
To configure the project into a new build directory, run:
```
cmake -S path/to/source -B path/to/build -G"MinGW Makefiles"
```
To build it, run:
```
cmake --build path/to/build
```
this will create an executable in the root of the build folder