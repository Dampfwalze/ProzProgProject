#include "menu_manager.h"
#include "application_manager.h"
#include "game_util.h"
#include "asset_manager.h"
#include "render.h"
#include "event_dispatcher.h"

#include <stdio.h>

#define MENU_WIDTH 300
#define MENU_TOP 10
#define MENU_DIFFICULTY_BUTTON_HEIGHT 60
#define MENU_DIFFICULTY_BUTTON_PADDING 8

char menuShown;

SDL_Rect menuRect;
int rectCenter;

typedef struct
{
    int tileCountX;
    int tileCountY;
    int mineCount;
    const char *name;
} Difficulty;

#define difficultyCount sizeof(difficulties) / sizeof(Difficulty)
Difficulty difficulties[] = {
    {9, 9, 10, "Beginner"},
    {16, 16, 40, "Easy"},
    {30, 16, 70, "Medium"},
    {30, 16, 99, "Hard"},
};

int menu_mouseButtonEventCallback(SDL_MouseButtonEvent *event)
{
    if (event->type == SDL_MOUSEBUTTONUP && event->button == SDL_BUTTON_LEFT)
    {
        SDL_Rect rect = {
            menuRect.x + BORDER_SIZE + TOPBAR_PADDING,
            menuRect.y + BORDER_SIZE * 2 + TOPBAR_HEIGHT + TOPBAR_PADDING,
            menuRect.w - (BORDER_SIZE + TOPBAR_PADDING) * 2,
            (MENU_DIFFICULTY_BUTTON_HEIGHT + TOPBAR_PADDING) * difficultyCount,
        };
        int x = event->x - rect.x;
        int y = event->y - rect.y;
        if (x >= 0 && x < rect.w && y >= 0 && y < rect.h)
        {
            if (y % (MENU_DIFFICULTY_BUTTON_HEIGHT + TOPBAR_PADDING) <= MENU_DIFFICULTY_BUTTON_HEIGHT)
            {
                y /= MENU_DIFFICULTY_BUTTON_HEIGHT + TOPBAR_PADDING;
                if (y < 0 || y >= difficultyCount)
                    return 0;
                application_QuitMenu();
                application_startGameWith(difficulties[y].tileCountX, difficulties[y].tileCountY, difficulties[y].mineCount);
                return RENDER_EVERYTHING;
            }
        }
    }
    return 0;
}

void menu_setup(SDL_Renderer *renderer)
{
    menuShown = 1;
    loadStaticMine(renderer, 15);

    add_MouseButton_EventCallback(menu_mouseButtonEventCallback);
}

void _renderMenuBackground(SDL_Renderer *renderer, int renderFlags);
void _renderTopbar(SDL_Renderer *renderer);
void _renderDifficulties(SDL_Renderer *renderer);

void menu_render(SDL_Renderer *renderer, int renderFlags)
{
    if (!menuShown)
        return;

    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    menuRect.x = width / 2 - MENU_WIDTH / 2;
    menuRect.y = MENU_TOP;
    menuRect.w = MENU_WIDTH;
    menuRect.h = BORDER_SIZE * 3 + TOPBAR_HEIGHT + (MENU_DIFFICULTY_BUTTON_HEIGHT + TOPBAR_PADDING) * difficultyCount + TOPBAR_PADDING;
    rectCenter = width / 2;

    _renderMenuBackground(renderer, renderFlags);
    _renderTopbar(renderer);
    _renderDifficulties(renderer);
}

void _renderDifficulties(SDL_Renderer *renderer)
{
    SDL_Texture **font = assets.fonts.text.digits;
    int fontCount = sizeof(assets.fonts.text.digits) / sizeof(SDL_Texture *);

    SDL_Texture *tileTex = generateTile(renderer, 20, 2);

    SDL_Rect rect = {
        menuRect.x + BORDER_SIZE + TOPBAR_PADDING,
        menuRect.y + BORDER_SIZE * 2 + TOPBAR_HEIGHT + TOPBAR_PADDING,
        menuRect.w - (BORDER_SIZE + TOPBAR_PADDING) * 2,
        MENU_DIFFICULTY_BUTTON_HEIGHT,
    };
    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    for (size_t i = 0; i < difficultyCount; i++)
    {
        SDL_RenderFillRect(renderer, &rect);

        renderText(renderer, font, fontCount, difficulties[i].name, rect.x + MENU_DIFFICULTY_BUTTON_PADDING, rect.y + MENU_DIFFICULTY_BUTTON_PADDING);

        // Render mine count
        char text[8];
        snprintf(text, 8, "%d", difficulties[i].mineCount);

        int textW, textH;
        textDimensions(font, fontCount, text, &textW, &textH);
        renderText(renderer, font, fontCount, text, rect.x + MENU_DIFFICULTY_BUTTON_PADDING, rect.y + rect.h - MENU_DIFFICULTY_BUTTON_PADDING - textH);

        SDL_Rect mine;
        SDL_QueryTexture(assets.symbols.mineStaticSize, NULL, NULL, &mine.w, &mine.h);
        mine.x = rect.x + 10 + textW;
        mine.y = rect.y + rect.h - 10 - textH / 2 - mine.h / 2;
        SDL_RenderCopy(renderer, assets.symbols.mineStaticSize, NULL, &mine);

        // Render board size
        snprintf(text, 8, "%dx%d", difficulties[i].tileCountX, difficulties[i].tileCountY);

        textDimensions(font, fontCount, text, &textW, &textH);
        renderText(renderer, font, fontCount, text, mine.x + mine.w + 20, rect.y + rect.h - MENU_DIFFICULTY_BUTTON_PADDING - textH);

        SDL_Rect tile;
        SDL_QueryTexture(tileTex, NULL, NULL, &tile.w, &tile.h);
        tile.x = mine.x + mine.w + 20 + textW + 5;
        tile.y = rect.y + rect.h - 10 - textH / 2 - tile.h / 2;
        SDL_RenderCopy(renderer, tileTex, NULL, &tile);

        rect.y += rect.h + TOPBAR_PADDING;
    }
}

void _renderTopbar(SDL_Renderer *renderer)
{
    const char *title = "New Game";
    SDL_Texture **font = assets.fonts.textTitle.digits;
    int fontCount = sizeof(assets.fonts.textTitle.digits) / sizeof(SDL_Texture *);
    int titleWidth, titleHeight;
    textDimensions(font, fontCount, title, &titleWidth, &titleHeight);
    renderText(renderer, font, fontCount, title, rectCenter - titleWidth / 2, menuRect.y + BORDER_SIZE + TOPBAR_HEIGHT / 2 - titleHeight / 2);
}

void _renderMenuBackground(SDL_Renderer *renderer, int renderFlags)
{
    if (renderFlags & RENDER_MENU_BACK)
    {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 100);
        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
        SDL_RenderFillRect(renderer, NULL);
    }

    SDL_SetRenderDrawColor(renderer, 190, 190, 190, 255);
    SDL_RenderFillRect(renderer, &menuRect);

    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    SDL_Rect topbarRect = {menuRect.x + BORDER_SIZE, menuRect.y + BORDER_SIZE, menuRect.w - 2 * BORDER_SIZE, TOPBAR_HEIGHT};
    SDL_RenderFillRect(renderer, &topbarRect);

    SDL_Rect bodyRect = {menuRect.x + BORDER_SIZE, menuRect.y + BORDER_SIZE * 2 + topbarRect.h, menuRect.w - 2 * BORDER_SIZE, menuRect.h - BORDER_SIZE * 3 - topbarRect.h};
    SDL_RenderFillRect(renderer, &bodyRect);
}

void menu_quit()
{
    menuShown = 0;
    remove_MouseButton_EventCallback(menu_mouseButtonEventCallback);
}
