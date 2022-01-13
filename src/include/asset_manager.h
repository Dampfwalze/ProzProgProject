#include <SDL.h>

typedef struct
{
    struct
    {
        SDL_Texture *happy;
        SDL_Texture *cool;
        SDL_Texture *curious;
        SDL_Texture *dead;
    } smiley;

    struct
    {
        SDL_Texture *mine;
        SDL_Texture *flag;
        SDL_Texture *digits[8];
    } symbols;

    struct
    {
        SDL_Texture *smiley;
        SDL_Texture *tile;
    } tiles;

} Assets;

extern Assets assets;

void loadSmiley(SDL_Renderer *renderer, int size);
void loadSymbols(SDL_Renderer *renderer, int size);
SDL_Texture *generateTile(SDL_Renderer *renderer, int size, int padding);
void genSmileyTile(SDL_Renderer *renderer, int size);
void genGameTile(SDL_Renderer *renderer, int size);
void debug_renderAll(SDL_Renderer *renderer);
