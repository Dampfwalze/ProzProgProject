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
        SDL_Texture *mineStaticSize;
    } symbols;

    struct
    {
        SDL_Texture *smiley;
        SDL_Texture *tile;
    } tiles;

    struct
    {
        struct
        {
            SDL_Texture *digits[10];
            SDL_Texture *minus;
            SDL_Texture *background;
        } sevenSigment;

        struct
        {
            SDL_Texture *digits[128];
        } text;

        struct
        {
            SDL_Texture *digits[128];
        } textTitle;

    } fonts;

} Assets;

extern Assets assets;

void renderText(SDL_Renderer *renderer, SDL_Texture **font, int fontCaracterCount, const char *text, int x, int y);
void textDimensions(SDL_Texture **font, int fontCaracterCount, const char *text, int *w, int *h);

void loadTextFont(SDL_Renderer *renderer, int size);
void loadTitleTextFont(SDL_Renderer *renderer, int size);
void load7Segment(SDL_Renderer *renderer, int size);
void loadSmiley(SDL_Renderer *renderer, int size);
void loadSymbols(SDL_Renderer *renderer, int size);
void loadStaticMine(SDL_Renderer *renderer, int size);
SDL_Texture *generateTile(SDL_Renderer *renderer, int size, int padding);
void genSmileyTile(SDL_Renderer *renderer, int size);
void genGameTile(SDL_Renderer *renderer, int size);
void debug_renderAll(SDL_Renderer *renderer);
