#include <SDL2/SDL.h>

void initEventHandling();

int handleEvents();

char keyPressed(SDL_Keycode keyCode);
char keyReleased(SDL_Keycode keyCode);
char keyDown(SDL_Keycode keyCode);