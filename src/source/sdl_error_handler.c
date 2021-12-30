#include <SDL2/SDL.h>
#include <stdio.h>

void handleSDLError(const char *stm, const char *file, int line)
{
    printf("\nSDL Error: %s\n  in: %s:%d,\nwhile executing:\n      %s\n", SDL_GetError(), file, line, stm);
    SDL_ClearError();
}