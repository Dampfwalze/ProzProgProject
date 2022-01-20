#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

int application_setup();
void application_doFrame();
void application_render(int renderFlags);
void application_quit();
void application_startGameWith(int tileCountX, int tileCountY, int mineCount);
void application_startMenu();
void application_QuitMenu();
