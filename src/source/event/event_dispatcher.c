#include <SDL.h>
#include "event_dispatcher.h"
#include "keyboard_event.h"
#include "linked_list.h"

LinkedList keyEventCallbacks = {0};
LinkedList mouseButtonEventCallbacks = {0};
LinkedList mouseMotionEventCallbacks = {0};
LinkedList windowEventCallbacks = {0};
LinkedList userEventCallbacks = {0};

void initEventHandling()
{
    keyboardEvent_init();
}

void add_Key_EventCallback(KeyEventCallback callback) { linkedList_add(&keyEventCallbacks, callback); }
void add_MouseButton_EventCallback(MouseButtonEventCallback callback) { linkedList_add(&mouseButtonEventCallbacks, callback); }
void add_MouseMotion_EventCallback(MouseMotionEventCallback callback) { linkedList_add(&mouseMotionEventCallbacks, callback); }
void add_Window_EventCallback(WindowEventCallback callback) { linkedList_add(&windowEventCallbacks, callback); }
void add_User_EventCallback(UserEventCallback callback) { linkedList_add(&userEventCallbacks, callback); }

void remove_Key_EventCallback(KeyEventCallback callback) { linkedList_removeAll(&keyEventCallbacks, callback); }
void remove_MouseButton_EventCallback(MouseButtonEventCallback callback) { linkedList_remove(&mouseButtonEventCallbacks, callback); }
void remove_MouseMotion_EventCallback(MouseMotionEventCallback callback) { linkedList_remove(&mouseMotionEventCallbacks, callback); }
void remove_Window_EventCallback(WindowEventCallback callback) { linkedList_remove(&windowEventCallbacks, callback); }
void remove_User_EventCallback(UserEventCallback callback) { linkedList_remove(&userEventCallbacks, callback); }

int _callAll(void *event, LinkedList *list)
{
    int redraw = 0;
    void *current = linkedList_begin(list);
    while (current != NULL)
    {
        EventCallback c = current;
        current = linkedList_next(list);
        redraw = redraw | c(event);
    }
    return redraw;
}

int handleEvents(int *redraw)
{
    int shouldClose = 0;
    *redraw = 0;

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        int keyIndex;
        switch (event.type)
        {
        case SDL_QUIT:
            shouldClose = 1;
            break;
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            keyboardEvent_handle(&event);
            *redraw = *redraw | _callAll(&event, &keyEventCallbacks);
            break;
        case SDL_WINDOWEVENT:
            *redraw = *redraw | _callAll(&event, &windowEventCallbacks);
            break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            *redraw = *redraw | _callAll(&event.button, &mouseButtonEventCallbacks);
            break;
        case SDL_MOUSEMOTION:
            *redraw = *redraw | _callAll(&event.button, &mouseMotionEventCallbacks);
            break;
        case SDL_USEREVENT:
            *redraw = *redraw | _callAll(&event.user, &userEventCallbacks);
            break;
        default:
            break;
        }
    }
    return shouldClose;
}
