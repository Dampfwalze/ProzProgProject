#include "game_manager.h"
#include "game_math.h"
#include "render.h"
#include "game_util.h"
#include "asset_manager.h"
#include "event_dispatcher.h"

#include <stdio.h>

#define gameboardByteLength (boardSize.x * boardSize.y * sizeof(Tile))

#define SMILEY_HAPPY 0
#define SMILEY_COOL 1
#define SMILEY_CURIOUS 2
#define SMILEY_DEAD 3

#define GAMESTATE_RUNNING 0
#define GAMESTATE_LOST 1
#define GAMESTATE_WON 2

Vec2i boardSize = {16, 16};

Tile *gameBoard;

int tileSize = 25;

int mineMaxCount = 30;
int mineMarkedCount;

int revealedTiles;

int timer;

GameDimensions gameDimensions;

char generated;

char gameState;
char hover;

char smileyOnly;

int smileyState;

SDL_Rect smileyRect;

SDL_Point actionTile;

void _renderBackground(SDL_Renderer *renderer);
void _renderBoard(SDL_Renderer *renderer);
void _renderSmilie(SDL_Renderer *renderer);
void _renderMineCounter(SDL_Renderer *renderer);
void _renderTimer(SDL_Renderer *renderer);

SDL_TimerID timerID;

Uint32 timerCallback(Uint32 interval, void *param)
{
    timer += 1;

    SDL_UserEvent userEvent;
    userEvent.type = SDL_USEREVENT;
    userEvent.timestamp = timer;
    userEvent.code = EVENT_GAME_TIMER;

    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user = userEvent;
    SDL_PushEvent(&event);

    if (timer >= 999)
        return 0;
    return 1000;
}

int game_userEventCallback(SDL_UserEvent *event)
{
    switch (event->code)
    {
    case EVENT_GAME_TIMER:
        return RENDER_TIMER;
    default:
        return 0;
    }
}

int game_windowEventCallback(SDL_WindowEvent *event)
{
    SDL_Renderer *renderer;
    switch (event->event)
    {
    case SDL_WINDOWEVENT_SIZE_CHANGED:
    case SDL_WINDOWEVENT_RESIZED:
        renderer = SDL_GetRenderer(SDL_GetWindowFromID(event->windowID));
        game_sizeChanged(renderer);
        return RENDER_FULLGAME;
    default:
        return 0;
    }
}

int game_mouseMotionEventCallback(SDL_MouseMotionEvent *event)
{
    if (gameState == GAMESTATE_RUNNING && hover)
    {
        actionTile.x = (event->x - gameDimensions.boardLeft) / gameDimensions.tileSize;
        actionTile.y = (event->y - gameDimensions.boardTop) / gameDimensions.tileSize;
        return RENDER_BOARD;
    }
    return 0;
}

int game_mouseButtonEventCallback(SDL_MouseButtonEvent *event)
{
    if (!smileyOnly && event->x >= gameDimensions.boardLeft && event->x < gameDimensions.boardRight && event->y >= gameDimensions.boardTop && event->y < gameDimensions.boardBottom)
        switch (event->button)
        {
        case SDL_BUTTON_LEFT:
        {
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                actionTile.x = (event->x - gameDimensions.boardLeft) / gameDimensions.tileSize;
                actionTile.y = (event->y - gameDimensions.boardTop) / gameDimensions.tileSize;
                hover = 1;

                smileyState = SMILEY_CURIOUS;
                return RENDER_SMILIE | RENDER_BOARD;
            }
            else
            {
                int x = (event->x - gameDimensions.boardLeft) / gameDimensions.tileSize;
                int y = (event->y - gameDimensions.boardTop) / gameDimensions.tileSize;
                hover = 0;
                if (!generated)
                {
                    generateMines(gameBoard, boardSize.x, boardSize.y, mineMaxCount, x, y);
                    timerID = SDL_AddTimer(1000, timerCallback, NULL);
                    generated = 1;
                }

                if (revealTile(gameBoard, boardSize.x, boardSize.y, x, y)) // loses
                {
                    actionTile.x = x;
                    actionTile.y = y;
                    smileyState = SMILEY_DEAD;
                    game_lose();
                }
                else if (revealedTiles == boardSize.x * boardSize.y - mineMaxCount) // Wins
                {
                    smileyState = SMILEY_COOL;
                    game_win();
                }
                else
                    smileyState = SMILEY_HAPPY;

                return RENDER_SMILIE | RENDER_BOARD;
            }
        }
        case SDL_BUTTON_RIGHT:
            if (event->type == SDL_MOUSEBUTTONDOWN)
            {
                int x = (event->x - gameDimensions.boardLeft) / gameDimensions.tileSize;
                int y = (event->y - gameDimensions.boardTop) / gameDimensions.tileSize;

                Tile *tile = &gameBoard[x + boardSize.x * y];
                *tile = (*tile & MARKED_MASK) ? *tile & ~MARKED_MASK : *tile | MARKED_MASK;
                mineMarkedCount += (*tile & MARKED_MASK) ? -1 : 1;
                return RENDER_BOARD | RENDER_MINECOUNTER;
            }
            return 0;
        }

    if (event->type == SDL_MOUSEBUTTONUP && event->x >= smileyRect.x && event->y >= smileyRect.y && event->x <= smileyRect.x + smileyRect.w && event->y <= smileyRect.y + smileyRect.h)
    {
        printf("Smiley clicked!\n");

        SDL_Renderer *renderer;
        switch (event->button)
        {
        case SDL_BUTTON_LEFT:
            application_startMenu();
            return RENDER_MENU;
        case SDL_BUTTON_RIGHT:
            renderer = SDL_GetRenderer(SDL_GetWindowFromID(event->windowID));
            game_restart(renderer);
            return RENDER_FULLGAME;
        }
    }
    return 0;
}

void game_setParameter(int tileCountX, int tileCountY, int mineCount)
{
    boardSize.x = tileCountX;
    boardSize.y = tileCountY;
    mineMaxCount = mineCount;
}

void game_restart(SDL_Renderer *renderer)
{
    game_quit();
    game_setup(renderer);
}

// Gets called when game starts and when ever the window size changes
void game_sizeChanged(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    calcGameDimensions(&gameDimensions, width, height, boardSize.x, boardSize.y);

    game_loadAssets(renderer);
}

// Only called from game_sizeChanged
void game_loadAssets(SDL_Renderer *renderer)
{
    loadSymbols(renderer, gameDimensions.tileSize * 0.7f);
    genGameTile(renderer, gameDimensions.tileSize);
}

void game_loadStaticAssets(SDL_Renderer *renderer)
{
    // Load Assets
    loadSmiley(renderer, 17); // leave at 17 and use point filtering, when rendering (default)
    genSmileyTile(renderer, TOPBAR_CONTENT_HEIGHT);
    load7Segment(renderer, TOPBAR_CONTENT_HEIGHT * 1.2);
}

void game_setup(SDL_Renderer *renderer)
{
    add_Window_EventCallback(game_windowEventCallback);
    add_MouseButton_EventCallback(game_mouseButtonEventCallback);
    add_MouseMotion_EventCallback(game_mouseMotionEventCallback);
    add_User_EventCallback(game_userEventCallback);

    game_sizeChanged(renderer);

    // Alloc game board
    gameBoard = malloc(gameboardByteLength);
    SDL_memset(gameBoard, 0, gameboardByteLength);

    generated = 0;
    timer = 0;
    mineMarkedCount = mineMaxCount;
    revealedTiles = 0;
    smileyOnly = 0;
    smileyState = SMILEY_HAPPY;
    gameState = GAMESTATE_RUNNING;
    actionTile.x = -1;
    actionTile.y = -1;
}

void game_win()
{
    remove_User_EventCallback(game_userEventCallback);
    remove_MouseMotion_EventCallback(game_mouseMotionEventCallback);

    SDL_RemoveTimer(timerID);

    smileyOnly = 1;

    for (size_t i = 0; i < boardSize.x * boardSize.y; i++)
        if (gameBoard[i] & MINE_MASK)
            gameBoard[i] = gameBoard[i] | MARKED_MASK;

    gameState = GAMESTATE_WON;
}

void game_lose()
{
    remove_User_EventCallback(game_userEventCallback);
    remove_MouseMotion_EventCallback(game_mouseMotionEventCallback);

    SDL_RemoveTimer(timerID);

    smileyOnly = 1;

    for (size_t i = 0; i < boardSize.x * boardSize.y; i++)
        if (gameBoard[i] & MINE_MASK)
            gameBoard[i] = gameBoard[i] | REVEALED_MASK;

    gameState = GAMESTATE_LOST;
}

void game_pause()
{
    remove_MouseButton_EventCallback(game_mouseButtonEventCallback);
    remove_User_EventCallback(game_userEventCallback);
    remove_MouseMotion_EventCallback(game_mouseMotionEventCallback);
}
void game_resume()
{
    add_MouseButton_EventCallback(game_mouseButtonEventCallback);
    add_User_EventCallback(game_userEventCallback);
    add_MouseMotion_EventCallback(game_mouseMotionEventCallback);
}

void game_quit()
{
    remove_Window_EventCallback(game_windowEventCallback);
    remove_MouseButton_EventCallback(game_mouseButtonEventCallback);
    remove_MouseMotion_EventCallback(game_mouseMotionEventCallback);
    remove_User_EventCallback(game_userEventCallback);

    SDL_RemoveTimer(timerID);

    free(gameBoard);
}

void game_render(SDL_Renderer *renderer, int renderFlags)
{
    if (renderFlags & RENDER_BOARD && renderFlags & RENDER_SMILIE && renderFlags & RENDER_MINECOUNTER && renderFlags & RENDER_TIMER)
        _renderBackground(renderer);
    if (renderFlags & RENDER_BOARD)
        _renderBoard(renderer);
    if (renderFlags & RENDER_SMILIE)
        _renderSmilie(renderer);
    if (renderFlags & RENDER_MINECOUNTER)
        _renderMineCounter(renderer);
    if (renderFlags & RENDER_TIMER)
        _renderTimer(renderer);

    //debug_renderAll(renderer);
}

void _renderBackground(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderClear(renderer);

    // Topbar background
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect topbarRect = {
        BORDER_SIZE,
        BORDER_SIZE,
        width - 2 * BORDER_SIZE,
        TOPBAR_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &topbarRect);

    // Board background
    SDL_Rect boardRect = {
        BORDER_SIZE,
        2 * BORDER_SIZE + TOPBAR_HEIGHT,
        width - 2 * BORDER_SIZE,
        height - 3 * BORDER_SIZE - TOPBAR_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &boardRect);
}

void _renderBoard(SDL_Renderer *renderer)
{
    // Background
    SDL_Rect backRect = {
        gameDimensions.boardLeft,
        gameDimensions.boardTop,
        gameDimensions.boardRight - gameDimensions.boardLeft,
        gameDimensions.boardBottom - gameDimensions.boardTop,
    };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &backRect);

    // Background Lines
    SDL_SetRenderDrawColor(renderer, 110, 110, 110, 255);
    for (size_t i = 0; i <= boardSize.x; i++)
    {
        int x = i * gameDimensions.tileSize + gameDimensions.boardLeft;
        SDL_RenderDrawLine(renderer, x, gameDimensions.boardTop, x, gameDimensions.boardBottom);
    }
    for (size_t i = 0; i <= boardSize.y; i++)
    {
        int y = i * gameDimensions.tileSize + gameDimensions.boardTop;
        SDL_RenderDrawLine(renderer, gameDimensions.boardLeft, y, gameDimensions.boardRight, y);
    }

    // Tiles
    SDL_Rect mineRect, flagRect;
    SDL_QueryTexture(assets.symbols.mine, NULL, NULL, &mineRect.w, &mineRect.h);
    SDL_QueryTexture(assets.symbols.flag, NULL, NULL, &flagRect.w, &flagRect.h);

    SDL_Rect rect = {0, 0, gameDimensions.tileSize, gameDimensions.tileSize};

    for (size_t i = 0; i < boardSize.x; i++)
    {
        for (size_t j = 0; j < boardSize.y; j++)
        {
            Tile tile = gameBoard[j * boardSize.x + i];
            rect.x = gameDimensions.boardLeft + i * gameDimensions.tileSize;
            rect.y = gameDimensions.boardTop + j * gameDimensions.tileSize;

            if (gameState == GAMESTATE_LOST && tile & MARKED_MASK && !(tile & MINE_MASK))
            {
                // Render mine
                mineRect.x = rect.x + rect.w / 2 - mineRect.w / 2;
                mineRect.y = rect.y + rect.h / 2 - mineRect.h / 2;
                SDL_RenderCopy(renderer, assets.symbols.mine, NULL, &mineRect);

                // Render cross
                float scale = 5;
                SDL_Point line1[] = {
                    {(rect.x + 2) / scale, (rect.y + 2) / scale},
                    {(rect.x + rect.w - 2) / scale, (rect.y + rect.h - 2) / scale},
                };
                SDL_Point line2[] = {
                    {line1[1].x, line1[0].y},
                    {line1[0].x, line1[1].y},
                };
                SDL_RenderSetScale(renderer, scale, scale);
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                SDL_RenderDrawLines(renderer, line1, 2);
                SDL_RenderDrawLines(renderer, line2, 2);
                //SDL_RenderDrawLine(renderer, line1[0].x, line1[0].y, line1[1].x, line1[1].y);
                //SDL_RenderDrawLine(renderer, line2[0].x, line2[0].y, line2[1].x, line2[1].y);
                //SDL_RenderDrawLine(renderer, rect.x + 2, rect.y + 2, rect.x + rect.w - 2, rect.y + rect.h - 2);
                //SDL_RenderDrawLine(renderer, rect.x + rect.w - 2, rect.y + 2, rect.x + 2, rect.y + rect.h - 2);
                SDL_RenderSetScale(renderer, 1, 1);
            }

            else if (!(tile & REVEALED_MASK))
            {
                // Render tile
                if (gameState != GAMESTATE_RUNNING || !hover || actionTile.x != i || actionTile.y != j)
                    SDL_RenderCopy(renderer, assets.tiles.tile, NULL, &rect);

                // Render flag
                if (tile & MARKED_MASK)
                {
                    flagRect.x = rect.x + rect.w / 2 - flagRect.w / 2;
                    flagRect.y = rect.y + rect.h / 2 - flagRect.h / 2;
                    SDL_RenderCopy(renderer, assets.symbols.flag, NULL, &flagRect);
                }
            }
            else
            {
                if (tile & MINE_MASK)
                {
                    if (gameState == GAMESTATE_LOST && actionTile.x == i && actionTile.y == j)
                    {
                        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
                        SDL_RenderFillRect(renderer, &rect);
                    }

                    // Render mine
                    mineRect.x = rect.x + rect.w / 2 - mineRect.w / 2;
                    mineRect.y = rect.y + rect.h / 2 - mineRect.h / 2;
                    SDL_RenderCopy(renderer, assets.symbols.mine, NULL, &mineRect);
                }
                else
                {
                    // Render digit
                    unsigned int sym = (tile & SYMBOL_MASK) >> 2;
                    if (sym > 0)
                    {
                        SDL_Texture *symTex = assets.symbols.digits[sym - 1];
                        SDL_Rect symRect;
                        SDL_QueryTexture(symTex, NULL, NULL, &symRect.w, &symRect.h);
                        symRect.x = rect.x + rect.w / 2 - symRect.w / 2;
                        symRect.y = rect.y + rect.h / 2 - symRect.h / 2;
                        SDL_RenderCopy(renderer, symTex, NULL, &symRect);
                    }
                }
            }
        }
    }
}

void _renderSmilie(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    // Background tile
    SDL_QueryTexture(assets.tiles.smiley, NULL, NULL, &smileyRect.w, &smileyRect.h);
    smileyRect.x = width / 2 - smileyRect.w / 2;
    smileyRect.y = BORDER_SIZE + TOPBAR_HEIGHT / 2 - smileyRect.h / 2;
    SDL_RenderCopy(renderer, assets.tiles.smiley, NULL, &smileyRect);

    // Smiley
    SDL_Rect texRect = {
        smileyRect.x + 5,
        smileyRect.y + 5,
        smileyRect.w - 10,
        smileyRect.h - 10,
    };
    SDL_RenderCopy(renderer, (&assets.smiley.happy)[smileyState], NULL, &texRect);
}

#define DIGITAL_DISP_WIDTH 100

void _renderDigitalDisp(SDL_Renderer *renderer, int value, int posX, int posY)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    // Background
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_Rect backRect = {
        posX,
        posY,
        DIGITAL_DISP_WIDTH,
        TOPBAR_CONTENT_HEIGHT,
    };
    SDL_RenderFillRect(renderer, &backRect);

    // Convert value to string
    char str[4];
    snprintf(str, 4, "%.3d", value < 0 ? -value : value);
    for (int i = 0; i < 3; i++)
        str[i] = clamp('0', '0' + 10, str[i]);
    if (value < 0)
        str[0] = '-';

    // Render digits
    SDL_Rect backDigitRect = {0, posY, 0, 0};
    SDL_QueryTexture(assets.fonts.sevenSigment.background, NULL, NULL, &backDigitRect.w, &backDigitRect.h);
    backDigitRect.y += TOPBAR_CONTENT_HEIGHT / 2 - backDigitRect.h / 2;
    for (size_t i = 0; i < 3; i++)
    {
        backDigitRect.x = posX + i * (DIGITAL_DISP_WIDTH / 3);
        SDL_RenderCopy(renderer, assets.fonts.sevenSigment.background, NULL, &backDigitRect);
        if (str[i] != '-')
            SDL_RenderCopy(renderer, assets.fonts.sevenSigment.digits[str[i] - '0'], NULL, &backDigitRect);
        else
        {
            SDL_Rect minus;
            SDL_QueryTexture(assets.fonts.sevenSigment.minus, NULL, NULL, &minus.w, &minus.h);
            minus.x = backDigitRect.x + backDigitRect.w / 2 - minus.w / 2;
            minus.y = backDigitRect.y + backDigitRect.h / 2 - minus.h / 2;
            SDL_RenderCopy(renderer, assets.fonts.sevenSigment.minus, NULL, &minus);
        }
    }
}

void _renderMineCounter(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    _renderDigitalDisp(renderer, mineMarkedCount, TOPBAR_PADDING + BORDER_SIZE, BORDER_SIZE + TOPBAR_PADDING);
}

void _renderTimer(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    _renderDigitalDisp(renderer, timer, width - DIGITAL_DISP_WIDTH - TOPBAR_PADDING - BORDER_SIZE, BORDER_SIZE + TOPBAR_PADDING);
}