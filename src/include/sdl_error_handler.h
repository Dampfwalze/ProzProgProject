void handleSDLError(const char *stm, const char *file, int line);

#ifdef __GNUC__

#define handle_sdl_error(stm)                                  \
    (                                                          \
        {                                                      \
            SDL_ClearError();                                  \
            typeof(stm) v = stm;                               \
            if ((v != 0) == (_Generic(v, int: 1, default: 0))) \
                handleSDLError(#stm, __FILE__, __LINE__);      \
            v;                                                 \
        })

#else

#define handle_sdl_error(stm) stm

#endif
