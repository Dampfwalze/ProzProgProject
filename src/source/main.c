#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

#include "game.h"
#include "event_dispatcher.h"
#include "sdl_error_handler.h"

int main(int argc, char* args[])
{
    if (handle_sdl_error(SDL_Init(SDL_INIT_EVERYTHING)))
    {
        printf("SDL_Init failed!\n");
        return EXIT_FAILURE;
    }

    SDL_Window *window = handle_sdl_error(SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    SDL_Renderer *renderer = handle_sdl_error(SDL_CreateRenderer(window, -1, 0));

    initEventHandling();

    // Game setup
    setup();

    while (!handleEvents())
    {
        SDL_Delay(16); // temporary, results in approximately 60 FPS

        // Do game frame
        doFrame(renderer);

        SDL_RenderPresent(renderer);
    }

    // Game quit
    quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();
    return 0;
}