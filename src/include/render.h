

#define RENDER_BOARD        0b10000000
#define RENDER_SMILIE       0b01000000
#define RENDER_MINECOUNTER  0b00100000
#define RENDER_TIMER        0b00010000
#define RENDER_FULLGAME     0b11110000
#define RENDER_MENU         0b00001000
#define RENDER_EVERYTHING   0b11111111

///**
// * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
// */
//#define setColor(r, g, b, a) SDL_SetRenderDrawColor(renderer, r, g, b, a)
//
///**
// * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
// */
//#define clearSurface() SDL_RenderClear(renderer)
//
///**
// * Requires a pointer to a SDL_Renderer structure called "renderer" to be present in the current context
// */
//#define drawRect(x, y, w, h)                                      \
//    (                                                             \
//        {                                                         \
//            _Generic(x,                                           \
//                     int                                          \
//                     : (                                          \
//                           {                                      \
//                               SDL_Rect r = {x, y, w, h};         \
//                               SDL_RenderFillRect(renderer, &r);  \
//                           }),                                    \
//                       float                                      \
//                     : (                                          \
//                           {                                      \
//                               SDL_FRect r = {x, y, w, h};        \
//                               SDL_RenderFillRectF(renderer, &r); \
//                           }),                                    \
//                       double                                     \
//                     : (                                          \
//                         {                                        \
//                             SDL_FRect r = {x, y, w, h};          \
//                             SDL_RenderFillRectF(renderer, &r);   \
//                         }));                                     \
//        })