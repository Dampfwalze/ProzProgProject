#include "application_manager.h"
#include "game_manager.h"
#include "sdl_error_handler.h"
#include "render.h"

SDL_Window *window;
SDL_Renderer *renderer;

void application_setup()
{
    if (handle_sdl_error(SDL_Init(SDL_INIT_EVERYTHING)))
    {
        printf("SDL_Init failed!\n");
        return EXIT_FAILURE;
    }
    if (handle_sdl_error(TTF_Init()))
    {
        printf("TTF_Init failed!\n");
        return EXIT_FAILURE;
    }
    //if (handle_sdl_error(IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
    //{
    //    printf("IMG_Init failed!\n");
    //    return EXIT_FAILURE;
    //}

    // window and renderer creation
    window = handle_sdl_error(SDL_CreateWindow("Test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 400, 300, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    renderer = handle_sdl_error(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC));

    // Game setup
    game_setup(renderer);
}

void application_render(int renderFlags)
{
    if (renderFlags & RENDER_FULLGAME)
        game_render(renderer, renderFlags);

    SDL_RenderPresent(renderer);
}

void application_quit()
{
    // Game quit
    game_quit();

    // destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    //IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}