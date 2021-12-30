/**
 * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
 */
#define setColor(r, g, b, a) SDL_SetRenderDrawColor(renderer, r, g, b, a)

/**
 * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
 */
#define clearSurface() SDL_RenderClear(renderer)

/**
 * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
 */
#define drawRect(x, y, w, h)                                      \
    (                                                             \
        {                                                         \
            _Generic(x,                                           \
                     int                                          \
                     : (                                          \
                           {                                      \
                               SDL_Rect r = {x, y, w, h};         \
                               SDL_RenderFillRect(renderer, &r);  \
                           }),                                    \
                       float                                      \
                     : (                                          \
                           {                                      \
                               SDL_FRect r = {x, y, w, h};        \
                               SDL_RenderFillRectF(renderer, &r); \
                           }),                                    \
                       double                                     \
                     : (                                          \
                         {                                        \
                             SDL_FRect r = {x, y, w, h};          \
                             SDL_RenderFillRectF(renderer, &r);   \
                         }));                                     \
        })