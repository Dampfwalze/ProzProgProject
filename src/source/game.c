#include "game.h"

void setup()
{
}

void quit()
{
}

void doFrame(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_RenderClear(renderer);
}