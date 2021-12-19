# ProzProgProject

## How to run
Makefile has 4 targets for 4 different cases:
- win-debug
- win-release
- unix-debug
- unix-release

Run them with `make target` on unix based systems and `mingw32-make target` with mingw on windows, where `target` is one of the specified targets. Output will be placed into `/bin`.

To add additional include directories, add them to:
- `COMMON_INCLUDES`, when they are the same for all plattforms
- `WIN_INCLUDES` and `UNIX_INCLUDES`, when they are different for each plattform

To add additional binarys to the linker, add them to
- `COMMON_LFLAGS`, when they are the same for all plattforms
- `WIN_LFLAGS` and `UNIX_LFLAGS`, when they are different for each plattform