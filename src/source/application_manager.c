#include "application_manager.h"
#include "game_manager.h"
#include "sdl_error_handler.h"
#include "render.h"
#include "event_dispatcher.h"
#include "asset_manager.h"

#include <stdio.h>
#include <stdlib.h>

#define TH 16
#define TW 16
#define W (20+TW+1+(TW*25))
#define H (105+TH+1+(TH*25))

SDL_Window *window;
SDL_Renderer *renderer;

SDL_Texture *image;

int windowEventCallback(SDL_WindowEvent *event)
{
    switch (event->event)
    {
    case SDL_WINDOWEVENT_RESIZED:
        resizeAll(window);
        return RENDER_EVERYTHING;
    default:
        return 0;
    }
}

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

    // Game setup
    game_setup(renderer);

    //image = IMG_LoadTexture(renderer, "resource/textures/test.jpg");
    //game_render(renderer, RENDER_BOARD);

    // Load Assets
    loadSmiley(renderer, 17); // leave at 17 and use point filtering, when rendering (default)
    genSmileyTile(renderer, 50);
    game_loadAssets(renderer);

    add_Window_EventCallback(windowEventCallback);

    return 0;
}

void application_render(int renderFlags)
{
    if (renderFlags & RENDER_FULLGAME)
        game_render(renderer, renderFlags);

    //SDL_Rect rect = {0};
    //SDL_QueryTexture(image, NULL, NULL, &rect.w, &rect.h);
    //SDL_RenderCopy(renderer, image, NULL, NULL);

    //debug_renderAll(renderer);

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