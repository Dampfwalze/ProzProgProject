# ProzProgProject

<p align="center">
<img src="img/preview.png" width="40%" alt="Preview" />
</p>

Minesweeper. This project is part of a university assignment for the module __"Prozedurale Programmierung"__ at __TUHH__.

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
This will create an executable at bin/app(.exe) in the root of the build folder
