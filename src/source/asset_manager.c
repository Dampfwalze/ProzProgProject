#include "asset_manager.h"

#include <stdio.h>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "sdl_error_handler.h"
#include "render.h"

#define SMILEY_PATH "resource/textures/smiley.png"
#define SMILEY_SIZE 409

#define SYMBOL_FONT_PATH "resource/font/mine-sweeper.ttf"

Assets assets;

SDL_Color digitColors[] = {
    {0x00, 0x00, 0xff, 0xff},
    {0x00, 0x7b, 0x00, 0xff},
    {0xff, 0x00, 0x00, 0xff},
    {0x00, 0x00, 0x7b, 0xff},
    {0x7b, 0x00, 0x00, 0xff},
    {0x00, 0x7b, 0x7b, 0xff},
    {0x00, 0x00, 0x00, 0xff},
    {0x00, 0x00, 0x00, 0xff},
};

SDL_Texture *_renderSmiley(SDL_Renderer *renderer, SDL_Rect *srcRect, SDL_Texture *src, int size, Uint32 format)
{
    SDL_Texture *smiley = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetTextureBlendMode(smiley, SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, smiley);
    SDL_RenderCopy(renderer, src, srcRect, NULL);

    return smiley;
}

void loadSmiley(SDL_Renderer *renderer, int size)
{
    SDL_Texture *smiley = IMG_LoadTexture(renderer, SMILEY_PATH);

    Uint32 format;
    int w, h;
    SDL_QueryTexture(smiley, &format, NULL, &w, &h);

    SDL_Rect srcRect;
    srcRect.w = SMILEY_SIZE;
    srcRect.h = SMILEY_SIZE;

    srcRect.x = 0;
    srcRect.y = 0;
    SDL_Texture *smiley_happy = _renderSmiley(renderer, &srcRect, smiley, size, format);

    srcRect.x = 0;
    srcRect.y = h - SMILEY_SIZE;
    SDL_Texture *smiley_cool = _renderSmiley(renderer, &srcRect, smiley, size, format);

    srcRect.x = w - SMILEY_SIZE;
    srcRect.y = 0;
    SDL_Texture *smiley_curious = _renderSmiley(renderer, &srcRect, smiley, size, format);

    srcRect.x = w - SMILEY_SIZE;
    srcRect.y = h - SMILEY_SIZE;
    SDL_Texture *smiley_dead = _renderSmiley(renderer, &srcRect, smiley, size, format);

    SDL_SetRenderTarget(renderer, NULL);

    if (assets.smiley.happy)
        SDL_DestroyTexture(assets.smiley.happy);
    if (assets.smiley.cool)
        SDL_DestroyTexture(assets.smiley.cool);
    if (assets.smiley.curious)
        SDL_DestroyTexture(assets.smiley.curious);
    if (assets.smiley.dead)
        SDL_DestroyTexture(assets.smiley.dead);

    assets.smiley.happy = smiley_happy;
    assets.smiley.cool = smiley_cool;
    assets.smiley.curious = smiley_curious;
    assets.smiley.dead = smiley_dead;
}

void loadSymbols(SDL_Renderer *renderer, int size)
{
    TTF_Font *font = handle_sdl_error(TTF_OpenFont(SYMBOL_FONT_PATH, size));

    // Digits
    for (int i = 0; i < 8; i++)
    {
        SDL_Surface *surface = handle_sdl_error(TTF_RenderGlyph_Blended(font, L'1' + i, digitColors[i]));
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        SDL_free(assets.symbols.digits[i]);
        assets.symbols.digits[i] = texture;

        int w, h;
        SDL_QueryTexture(texture, NULL, NULL, &w, &h);
        printf("width: %d, height: %d\n", w, h);
    }

    // Mine
    {
        SDL_Color col = {0, 0, 0, 0xff};
        SDL_Surface *surface = handle_sdl_error(TTF_RenderGlyph_Blended(font, L'*', col));
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int w, h;
        Uint32 format;
        SDL_QueryTexture(texture, &format, NULL, &w, &h);
        SDL_Texture *res = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetTextureBlendMode(res, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, res);

        SDL_Rect r = {
            4 * w / 14 - 1,
            4 * h / 14 - 1,
            2 * w / 14 + 2,
            2 * w / 14 + 2,
        };
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &r);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_SetRenderTarget(renderer, NULL);

        if (assets.symbols.mine)
            SDL_DestroyTexture(assets.symbols.mine);
        assets.symbols.mine = res;
    }

    // Flag
    {
        SDL_Color col = {255, 255, 255, 255};
        SDL_Surface *surface = handle_sdl_error(TTF_RenderGlyph_Blended(font, L'`', col));
        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);

        int w, h;
        Uint32 format;
        SDL_QueryTexture(texture, &format, NULL, &w, &h);
        SDL_Texture *res = SDL_CreateTexture(renderer, format, SDL_TEXTUREACCESS_TARGET, w, h);
        SDL_SetTextureBlendMode(res, SDL_BLENDMODE_BLEND);
        SDL_SetRenderTarget(renderer, res);

        SDL_SetTextureColorMod(texture, 0, 0, 0);

        SDL_RenderCopy(renderer, texture, NULL, NULL);

        SDL_SetTextureColorMod(texture, 255, 0, 0);
        SDL_Rect rect = {0, 0, w, h / 2};
        SDL_RenderCopy(renderer, texture, &rect, &rect);

        SDL_SetRenderTarget(renderer, NULL);

        if (assets.symbols.flag)
            SDL_DestroyTexture(assets.symbols.flag);
        assets.symbols.flag = res;
    }
}

void genSmileyTile(SDL_Renderer *renderer, int size)
{
    if (assets.tiles.smiley)
        SDL_DestroyTexture(assets.tiles.smiley);

    assets.tiles.smiley = generateTile(renderer, size, size / 20);
}

void genGameTile(SDL_Renderer *renderer, int size)
{
    if (assets.tiles.tile)
        SDL_DestroyTexture(assets.tiles.tile);

    assets.tiles.tile = generateTile(renderer, size, size / 10);
}

SDL_Texture *generateTile(SDL_Renderer *renderer, int size, int padding)
{
    SDL_Texture *texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, SDL_TEXTUREACCESS_TARGET, size, size);
    SDL_SetRenderTarget(renderer, texture);

    SDL_SetRenderDrawColor(renderer, 150, 150, 150, 255);
    SDL_RenderClear(renderer);

    SDL_Vertex verticies[] = {
        {0.0f, 0.0f, 255, 255, 255, 255, 0, 0},
        {size, 0.0f, 255, 255, 255, 255, 0, 0},
        {0.0f, size, 255, 255, 255, 255, 0, 0},
    };
    SDL_RenderGeometry(renderer, NULL, verticies, 3, NULL, 0);

    SDL_Rect rect = {
        padding,
        padding,
        size - padding * 2,
        size - padding * 2,
    };
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    SDL_RenderFillRect(renderer, &rect);

    SDL_SetRenderTarget(renderer, NULL);

    return texture;
}

void debug_renderAll(SDL_Renderer *renderer)
{
    int w, h;
    int yPos = 10;
    // Smilies
    {
        SDL_QueryTexture(assets.tiles.smiley, NULL, NULL, &w, &h);
        int maxHeight = h;
        SDL_Rect tileRect = {10, yPos, w, h};
        SDL_RenderCopy(renderer, assets.tiles.smiley, NULL, &tileRect);

        SDL_QueryTexture(assets.smiley.happy, NULL, NULL, &w, &h);
        if (h > maxHeight)
            maxHeight = h;
        SDL_Rect dstRect = {tileRect.x + tileRect.w + 10, yPos, w, h};
        for (size_t i = 0; i < 4; i++)
        {
            SDL_RenderCopy(renderer, (&assets.smiley.happy)[i], NULL, &dstRect);
            dstRect.x += w + 10;
        }
        yPos = maxHeight;
    }
    yPos += 10;

    // Digits
    {
        SDL_Rect dstRect = {10, yPos, 10, 10};
        int maxHeight = 0;
        for (size_t i = 0; i < 8; i++)
        {
            SDL_QueryTexture(assets.symbols.digits[i], NULL, NULL, &dstRect.w, &dstRect.h);
            SDL_RenderCopy(renderer, assets.symbols.digits[i], NULL, &dstRect);
            dstRect.x += dstRect.w + 10;
            if (maxHeight < dstRect.h)
                maxHeight = dstRect.h;
        }
        yPos += maxHeight;
    }
    yPos += 10;

    // Symbols
    {
        SDL_Rect dstRect = {10, yPos, 0, 0};
        SDL_QueryTexture(assets.tiles.tile, NULL, NULL, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(renderer, assets.tiles.tile, NULL, &dstRect);

        dstRect.x += dstRect.w + 10;
        SDL_QueryTexture(assets.symbols.flag, NULL, NULL, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(renderer, assets.symbols.flag, NULL, &dstRect);

        dstRect.x += dstRect.w + 10;
        SDL_QueryTexture(assets.symbols.mine, NULL, NULL, &dstRect.w, &dstRect.h);
        SDL_RenderCopy(renderer, assets.symbols.mine, NULL, &dstRect);
    }
}