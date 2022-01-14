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

//SDL_Rect tilesdeck[TH * TW];
SDL_Rect tiles[TH*TW], tilesnum[TH*TW];
SDL_Rect smiley, smileyback, topbarback, topbar;

//standard
int w_factor = 1, w_width = W, w_height = H, w_wborder = 10, w_hborder = 10;


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
            tiles[i * TW + j].x = tilesnum[i * TW + j].x = w_wborder+1 + (tileSize +1) * j;
            tiles[i * TW + j].y = tilesnum[i * TW + j].y = 2*w_hborder+3*tileSize+1 + (tileSize +1) * i;
            tiles[i * TW + j].h = tilesnum[i * TW + j].h = tileSize;
            tiles[i * TW + j].w = tilesnum[i * TW + j].w = tileSize;
        }
    }
    /*for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tilesnum[i * TW + j].x = w_border+1+ (tileSize*w_factor +1) * j;
            tilesnum[i * TW + j].y = 2*w_border+3*tileSize*w_factor+1+ (tileSize*w_factor +1) * i;
            tilesnum[i * TW + j].h = tileSize*w_factor;
            tilesnum[i * TW + j].w = tileSize*w_factor;
        }
    }*/
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
    topbarback.x = w_wborder;
    topbarback.y = w_hborder;
    topbarback.h = 3*tileSize;
    topbarback.w = w_width - 2 * w_wborder;
    topbar.x = w_wborder + (tileSize*0.08);
    topbar.y = w_hborder + (tileSize*0.08);
    topbar.h = 3*tileSize - (tileSize*0.08)*2;
    topbar.w = w_width - 2 * w_wborder -(tileSize*0.08)*2;
    /*SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_RenderFillRect(renderer, &topbar);*/

    // Smiley
    smileyback.x = (w_width / 2) - tileSize;
    smileyback.y = w_hborder + 3*tileSize*0.5-tileSize;
    smileyback.h = 2*tileSize;
    smileyback.w = 2*tileSize;
    /*SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &smileyback);*/
    smiley.x = (w_width / 2) - tileSize*0.6;
    smiley.y = w_hborder + 3*tileSize*0.5-tileSize*0.6;
    smiley.h = tileSize*1.2;
    smiley.w = tileSize*1.2;
    /*SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &smiley);*/

    // Game Tiles deck
    /*for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tilesdeck[i * TW + j].x = 11 + 26 * j;
            tilesdeck[i * TW + j].y = 96 + 26 * i;
            tilesdeck[i * TW + j].h = 25;
            tilesdeck[i * TW + j].w = 25;
        }
    }*/
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

void resizeAll(SDL_Window *window) {
    int wmaxsize, hmaxsize;
    SDL_GetWindowSize(window, &w_width, &w_height);
    wmaxsize = (w_width-20-(TW+1) ) / TW;
    hmaxsize = (w_height-30-(TH+1) ) / (TH+3);
    if (wmaxsize > hmaxsize) {
        tileSize = hmaxsize;
    } else tileSize = wmaxsize;
    w_height-= (w_height-( (TH+3)*tileSize + (TH+1) ) ) % 3;
    w_width-= (w_width-(TW+1 + (TW*tileSize))) % 2;
    w_hborder = (w_height-( (TH+3)*tileSize + (TH+1) ) ) / 3;
    w_wborder = (w_width-(TW+1 + (TW*tileSize))) / 2;

    //game tiles
    int i, j;
    for (i = 0; i <= TH-1; i++) {
        for (j = 0; j <= TW-1; j++) {
            tiles[i * TW + j].x = tilesnum[i * TW + j].x = w_wborder+1 + (tileSize +1) * j;
            tiles[i * TW + j].y = tilesnum[i * TW + j].y = 2*w_hborder+3*tileSize+1 + (tileSize +1) * i;
            tiles[i * TW + j].h = tilesnum[i * TW + j].h = tileSize;
            tiles[i * TW + j].w = tilesnum[i * TW + j].w = tileSize;
        }
    }

    // Smiley
    smileyback.x = (w_width / 2) - tileSize;
    smileyback.y = w_hborder + 3*tileSize*0.5-tileSize;
    smileyback.h = 2*tileSize;
    smileyback.w = 2*tileSize;
    smiley.x = (w_width / 2) - tileSize*0.6;
    smiley.y = w_hborder + 3*tileSize*0.5-tileSize*0.6;
    smiley.h = tileSize*1.2;
    smiley.w = tileSize*1.2;

    //topbar
    topbarback.x = w_wborder;
    topbarback.y = w_hborder;
    topbarback.h = 3*tileSize;
    topbarback.w = w_width - 2 * w_wborder;
    topbar.x = w_wborder + (tileSize*0.08);
    topbar.y = w_hborder + (tileSize*0.08);
    topbar.h = 3*tileSize - (tileSize*0.08)*2;
    topbar.w = w_width - 2 * w_wborder -(tileSize*0.08)*2;
}

void _renderBackground(SDL_Renderer *renderer)
{
    printf("Render background\n");
    
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 250, 250, 250, 255);
    SDL_RenderFillRect(renderer, &topbarback);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderFillRect(renderer, &topbar);
    
    SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);
    int i;
    for (i = 0; i <= TH; i++) {
        SDL_RenderDrawLine(renderer, w_wborder, 2*w_hborder+3*tileSize + (tileSize+1) * i, w_width-w_wborder-1, 2*w_hborder+3*tileSize + (tileSize+1) * i);
    }
    for (i = 0; i <= TW; i++) {
        SDL_RenderDrawLine(renderer, w_wborder + (tileSize+1) * i, 2*w_hborder+3*tileSize, w_wborder + (tileSize+1) * i, w_height-w_hborder-1);
    }
}

void _renderBoard(SDL_Renderer *renderer)
{
    printf("Render board\n");

    SDL_SetRenderDrawColor(renderer, 170, 170, 170, 255);
    SDL_RenderFillRects(renderer, tiles, TH*TW);
    

    //SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

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
        } else SDL_RenderCopy(renderer, assets.tiles.tile, NULL, &tiles[i]);
        if (gameBoard[i] & MARKED_MASK) {
            flag = tiles[i];
            flag.x += tileSize/5;
            flag.y += tileSize/5;
            flag.h -= tileSize/2.5;
            flag.w -= tileSize/2.5;
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