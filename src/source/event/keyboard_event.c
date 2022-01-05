#include "keyboard_event.h"

typedef char KeyState;

#define recordedKeyCodeCount (sizeof(recordedKeyCodes) / sizeof(SDL_Keycode))

SDL_Keycode recordedKeyCodes[] = {
    SDLK_a,
    SDLK_b,
    SDLK_c,
    SDLK_d,
    SDLK_e,
    SDLK_f,
    SDLK_g,
    SDLK_h,
    SDLK_i,
    SDLK_j,
    SDLK_k,
    SDLK_l,
    SDLK_m,
    SDLK_n,
    SDLK_o,
    SDLK_p,
    SDLK_q,
    SDLK_r,
    SDLK_s,
    SDLK_t,
    SDLK_u,
    SDLK_v,
    SDLK_w,
    SDLK_x,
    SDLK_y,
    SDLK_z,

    SDLK_0,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_4,
    SDLK_5,
    SDLK_6,
    SDLK_7,
    SDLK_8,
    SDLK_9,

    SDLK_RIGHT,
    SDLK_LEFT,
    SDLK_DOWN,
    SDLK_UP,

    SDLK_LCTRL,
    SDLK_LSHIFT,
    SDLK_LALT,
    SDLK_LGUI,
    SDLK_RCTRL,
    SDLK_RSHIFT,
    SDLK_RALT,
    SDLK_RGUI,

    SDLK_ESCAPE,
};

KeyState keyStates[recordedKeyCodeCount];

int _findKey(SDL_Keycode keyCode)
{
    SDL_Keycode *start = recordedKeyCodes;
    SDL_Keycode *end = start + recordedKeyCodeCount;
    SDL_Keycode *middle = start + recordedKeyCodeCount / 2;

    while (end - start > 1)
    {
        if (*middle == keyCode)
            return middle - recordedKeyCodes;
        else if (*middle < keyCode)
            start = middle;
        else
            end = middle;
        middle = start + (end - start) / 2;
    }
    return -1;
}

char keyPressed(SDL_Keycode keyCode)
{
    return keyStates[_findKey(keyCode)];
}

int _compare(const void *a, const void *b)
{
    if (*((int *)a) == *((int *)b))
        return 0;
    else if (*((int *)a) < *((int *)b))
        return -1;
    else
        return 1;
}

void keyboardEvent_init()
{
    qsort(recordedKeyCodes, recordedKeyCodeCount, sizeof(SDL_Keycode), _compare);
}

void keyboardEvent_handle(const SDL_Event *event)
{
    int keyIndex;
    switch (event->type)
    {
    case SDL_KEYDOWN:
        keyStates[_findKey(event->key.keysym.sym)] = 1;
        break;
    case SDL_KEYUP:
        keyStates[_findKey(event->key.keysym.sym)] = 0;
        break;
    default:
        break;
    }
}