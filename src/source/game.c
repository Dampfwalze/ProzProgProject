#include "game.h"
#include "game_math.h"
#include "render.h"

struct Player
{
    Vec2 position;
} player;

struct World
{
    double offsetToScreen;
} world;

void setup(int width, int height)
{
    memset(&player, 0, sizeof(player));
    player.position.x = width / 2;
}

void quit()
{
}

void doFrame(SDL_Renderer *renderer)
{
    int width, height;
    SDL_GetRendererOutputSize(renderer, &width, &height);

    setColor(255, 255, 255, 255);
    clearSurface();

    setColor(255, 0, 0, 255);
    drawRect(player.position.x - 5, (height - player.position.y) - 20, 10, 20);
}