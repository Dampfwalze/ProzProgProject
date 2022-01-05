#include <SDL.h>

void game_setup(SDL_Renderer *renderer);
void game_quit();
void game_doFrame(SDL_Renderer *renderer);
void game_render(SDL_Renderer *renderer, int renderFlags);
