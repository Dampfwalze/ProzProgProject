#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>

#include "application_manager.h"
#include "event_dispatcher.h"
#include "render.h"

int main(int argc, char *args[])
{
    int err = application_setup();
    if (err)
        return err;

    initEventHandling();

    // draw first frame
    application_render(RENDER_EVERYTHING);

    // main event loop
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