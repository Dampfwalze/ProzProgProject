#include "game_manager.h"
#include "game_math.h"
#include "render.h"
#include "game_util.h"
#include "asset_manager.h"

#include <stdio.h>

#define TH 16
#define TW 16
#define W (20+TW+1+(TW*25))
#define H (105+TH+1+(TH*25))

#define gameboardByteLength boardSize.x *boardSize.y * sizeof(Tile)

void _renderBackground(SDL_Renderer *renderer);
void _renderBoard(SDL_Renderer *renderer);
void _renderSmilie(SDL_Renderer *renderer);
void _renderMineCounter(SDL_Renderer *renderer);
void _renderTimer(SDL_Renderer *renderer);

Vec2 boardSize = {16, 16};

Tile *gameBoard;

int tileSize = 25;

SDL_Rect tilesdeck[TH * TW];
int tiledecked[TW*TH];
SDL_Rect tiles[TH*TW], tilesnum[TH*TW];
SDL_Rect smiley, smileyback, topbar;


void game_loadAssets(SDL_Renderer *renderer)
{
    loadSymbols(renderer, ((float)tileSize) * 0.8f);
    genGameTile(renderer, tileSize);
}

void game_setup(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    // Alloc game board
    gameBoard = malloc(gameboardByteLength);
    SDL_memset(gameBoard, 0, gameboardByteLength);

    // Background
    /*SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderClear(renderer);*/

    // Game Tiles
    int i, j;
    for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tiles[i * TW + j].x = 11 + 26 * j;
            tiles[i * TW + j].y = 96 + 26 * i;
            tiles[i * TW + j].h = 25;
            tiles[i * TW + j].w = 25;
        }
    }
    for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tilesnum[i * TW + j].x = 13 + 26 * j;
            tilesnum[i * TW + j].y = 98 + 26 * i;
            tilesnum[i * TW + j].h = 21;
            tilesnum[i * TW + j].w = 21;
        }
    }
    /*SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRects(renderer, tiles, TH*TW);*/

    // Game Tiles Outlines
    /*SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    for (i = 0; i <= TH; i++) {
        SDL_RenderDrawLine(renderer, 10, 95 + 26 * i, W-11, 95 + 26 * i);
    }
    for (i = 0; i <= TW; i++) {
        SDL_RenderDrawLine(renderer, 10 + 26 * i, 95, 10 + 26 * i, H-11);
    }*/

    // Topbar
    topbar.x = 10;
    topbar.y = 10;
    topbar.h = 75;
    topbar.w = W - 20;
    /*SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &topbar);*/

    // Smiley
    smileyback.x = (W / 2) - 27;
    smileyback.y = 20;
    smileyback.h = 55;
    smileyback.w = 55;
    /*SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &smileyback);*/
    smiley.x = (W / 2) - 15;
    smiley.y = 32;
    smiley.h = 30;
    smiley.w = 30;
    /*SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &smiley);*/

    // Game Tiles deck
    for (i = 0; i <= (TH*TW)-1; i++) {
        tiledecked[i] = 1;
    }
    for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tilesdeck[i * TW + j].x = 13 + 26 * j;
            tilesdeck[i * TW + j].y = 98 + 26 * i;
            tilesdeck[i * TW + j].h = 21;
            tilesdeck[i * TW + j].w = 21;
        }
    }
    /*SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRects(renderer, tilesdeck, TH*TW);*/
}

void game_quit()
{
}

void game_render(SDL_Renderer *renderer, int renderFlags)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

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
}

void _renderBackground(SDL_Renderer *renderer)
{
    printf("Render background\n");
    
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &topbar);
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    int i;
    for (i = 0; i <= TH; i++) {
        SDL_RenderDrawLine(renderer, 10, 95 + 26 * i, W-11, 95 + 26 * i);
    }
    for (i = 0; i <= TW; i++) {
        SDL_RenderDrawLine(renderer, 10 + 26 * i, 95, 10 + 26 * i, H-11);
    }
}

void _renderBoard(SDL_Renderer *renderer)
{
    printf("Render board\n");
    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRects(renderer, tiles, TH*TW);
    

    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    int i;

    for (i = 0; i <= (TW*TH)-1; i++) {
        if (gameBoard[i] & REVEALED_MASK) {
            if (gameBoard[i] & MINE_MASK) {
                SDL_RenderCopy(renderer, assets.symbols.mine, NULL, &tilesnum[i]);
            } else if (gameBoard[i] & SYMBOL_MASK)
                SDL_RenderCopy(renderer, assets.symbols.digits[((gameBoard[i] & SYMBOL_MASK)/4)], NULL, &tilesnum[i]);
        }
    }

    SDL_Rect flag;
    for (i = 0; i <= (TW*TH)-1; i++) {
        if (gameBoard[i] & REVEALED_MASK) {
        } else SDL_RenderCopy(renderer, assets.tiles.tile, NULL, &tilesdeck[i]);
        if (gameBoard[i] & MARKED_MASK) {
            flag = tiles[i];
            flag.x += 5;
            flag.y += 5;
            flag.h -= 10;
            flag.w -= 10;
            SDL_RenderCopy(renderer, assets.symbols.flag, NULL, &flag);
        }
    }

}

void _renderSmilie(SDL_Renderer *renderer)
{
    printf("Render smilie\n");

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    //SDL_RenderFillRect(renderer, &smileyback);
    SDL_RenderCopy(renderer, assets.tiles.smiley, NULL, &smileyback);
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    //SDL_RenderFillRect(renderer, &smiley);

    SDL_RenderCopy(renderer, assets.smiley.cool, NULL, &smiley);
}

void _renderMineCounter(SDL_Renderer *renderer)
{
    printf("Render mine counter\n");
}

void _renderTimer(SDL_Renderer *renderer)
{
    printf("Render timer\n");
}