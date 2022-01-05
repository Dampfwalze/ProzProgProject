#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "application_manager.h"
#include "event_dispatcher.h"
#include "render.h"

int keyBoardCallback(SDL_KeyboardEvent *event)
{
    printf("Key Event: %d\n", event->keysym.sym);
    switch (event->keysym.sym)
    {
    case SDLK_0:
        return RENDER_BOARD;

    case SDLK_1:
        return RENDER_SMILIE;

    case SDLK_2:
        return RENDER_MINECOUNTER;

    case SDLK_3:
        return RENDER_TIMER;

    case SDLK_4:
        return RENDER_FULLGAME;
    default:
        return 0;
    }
}

int main(int argc, char *args[])
{
    application_setup();

    initEventHandling();

    add_Key_EventCallback(keyBoardCallback);

    // draw first frame
    application_render(RENDER_EVERYTHING);

    // main game loop
    int redraw = 0;
    while (!handleEvents(&redraw))
    {
        if (redraw)
            application_render(redraw);
        SDL_Delay(2);
    }

    application_quit();

    return 0;
}