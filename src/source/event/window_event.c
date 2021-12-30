#include "window_event.h"

void windowEvent_init() {}

void windowEvent_beginFrame() {}
void windowEvent_handle(SDL_Event *event)
{
    // See SDL_WindowEventID for list of all events
    switch (event->window.event)
    {
    default:
        break;
    }
}