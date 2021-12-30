#include <SDL2/SDL.h>
#include "event_dispatcher.h"
#include "keyboard_event.h"
#include "window_event.h"

void initEventHandling()
{
    windowEvent_init();
    keyboardEvent_init();
}

int handleEvents()
{
    windowEvent_beginFrame();
    keyboardEvent_beginFrame();

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        int keyIndex;
        switch (event.type)
        {
        case SDL_QUIT:
            return 1;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyboardEvent_handle(&event);
            break;
        case SDL_WINDOWEVENT:
            windowEvent_handle(&event);
            break;
        default:
            break;
        }
    }
    return 0;
}
