#include "application_manager.h"
#include "game_manager.h"
#include "sdl_error_handler.h"
#include "render.h"

#include <stdio.h>
#include <stdlib.h>

#define TH 16
#define TW 16
#define W (20+TW+1+(TW*25))
#define H (105+TH+1+(TH*25))

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *image;

int application_setup()
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
    if (handle_sdl_error(!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)))
    {
        printf("IMG_Init failed!\n");
        return EXIT_FAILURE;
    }

    // window and renderer creation
    window = handle_sdl_error(SDL_CreateWindow("Minesweeper", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, W, H, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE));
    renderer = handle_sdl_error(SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC));

    printf("%s\n", SDL_GetError());

    // Game setup
    game_setup(renderer);
    
    printf("%s\n", SDL_GetError());

    //image = IMG_LoadTexture(renderer, "resource/textures/test.jpg");
    //game_render(renderer, RENDER_BOARD);
    printf("%s\n", SDL_GetError());

    return 0;
}

void application_render(int renderFlags)
{
    if (renderFlags & RENDER_FULLGAME)
        game_render(renderer, renderFlags);

    //SDL_Rect rect = {0};
    //SDL_QueryTexture(image, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, image, NULL, NULL);

    SDL_RenderPresent(renderer);
}

void application_quit()
{
    // Game quit
    game_quit();

    // destroy renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
}