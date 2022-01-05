#include "game_manager.h"
#include "game_math.h"
#include "render.h"

void _renderBoard(SDL_Renderer *renderer);
void _renderSmilie(SDL_Renderer *renderer);
void _renderMineCounter(SDL_Renderer *renderer);
void _renderTimer(SDL_Renderer *renderer);

void game_setup(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);
}

void game_quit()
{
}

void game_render(SDL_Renderer *renderer, int renderFlags)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    if (renderFlags & RENDER_BOARD)
        _renderBoard(renderer);
    if (renderFlags & RENDER_SMILIE)
        _renderSmilie(renderer);
    if (renderFlags & RENDER_MINECOUNTER)
        _renderMineCounter(renderer);
    if (renderFlags & RENDER_TIMER)
        _renderTimer(renderer);
}

void _renderBoard(SDL_Renderer *renderer)
{
    printf("Render board\n");
}

void _renderSmilie(SDL_Renderer *renderer)
{
    printf("Render smilie\n");
}

void _renderMineCounter(SDL_Renderer *renderer)
{
    printf("Render mine counter\n");
}

void _renderTimer(SDL_Renderer *renderer)
{
    printf("Render timer\n");
}