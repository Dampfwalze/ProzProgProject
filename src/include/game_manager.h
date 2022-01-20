#include <SDL.h>

extern int revealedTiles;

void game_loadAssets(SDL_Renderer *renderer);
void game_loadStaticAssets(SDL_Renderer *renderer);
void game_setup(SDL_Renderer *renderer);
void game_pause();
void game_resume();
void game_quit();
void game_render(SDL_Renderer *renderer, int renderFlags);
void game_sizeChanged(SDL_Renderer *renderer);
void game_setParameter(int tileCountX, int tileCountY, int mineCount);
