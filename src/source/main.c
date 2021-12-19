#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

int main(int argc, char* args[])
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
        printf("SDL_Init failed!\n");
    else
        printf("SDL_Init was successful!\n");

    SDL_Window* window = SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN);

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);

    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);

    SDL_RenderClear(renderer);

    SDL_RenderPresent(renderer);

    while (1)
    {
        SDL_Delay(1);
    }
    

    SDL_Quit();
    return 0;
}