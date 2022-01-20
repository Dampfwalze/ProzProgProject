#include <SDL.h>

typedef int (*EventCallback)(void *);
typedef int (*KeyEventCallback)(SDL_KeyboardEvent *);
typedef int (*MouseButtonEventCallback)(SDL_MouseButtonEvent *);
typedef int (*MouseMotionEventCallback)(SDL_MouseMotionEvent *);
typedef int (*WindowEventCallback)(SDL_WindowEvent *);
typedef int (*UserEventCallback)(SDL_UserEvent *);

void initEventHandling();

int handleEvents(int *redraw);

void add_Key_EventCallback(KeyEventCallback callback);
void add_MouseButton_EventCallback(MouseButtonEventCallback callback);
void add_MouseMotion_EventCallback(MouseMotionEventCallback callback);
void add_Window_EventCallback(WindowEventCallback callback);
void add_User_EventCallback(UserEventCallback callback);

void remove_Key_EventCallback(KeyEventCallback callback);
void remove_MouseButton_EventCallback(MouseButtonEventCallback callback);
void remove_MouseMotion_EventCallback(MouseMotionEventCallback callback);
void remove_Window_EventCallback(WindowEventCallback callback);
void remove_User_EventCallback(UserEventCallback callback);