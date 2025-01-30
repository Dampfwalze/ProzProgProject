#include "game_util.h"
#include "game_manager.h"
#include <time.h>
#include <stdlib.h>
#include <SDL.h>

void printBinary(uint64_t d)
{
    printf("Binary: ");
    for (int i = 0; i < sizeof(d) * 8; i++)
    {
        if (i != 0 && i % 8 == 0)
            printf(" ");
        uint64_t v = 1llu << i;
        printf((d & v) ? "1" : "0");
    }
    printf("\n");
}

/**
 * Generiert Minen, garantiert aber, dass in einem 3x3 Feld um ein bestimmtes Tile keine Mine generiert wird
 *
 * gameBoard - width * height großes array, in welches Minen generiert werden
 * width - Breite des Spielfelds
 * height - Höhe des Spielfelds
 * mineCount - Anzahl an zu generierenden Minen
 * avoidX - x-Koordinate des zu vermeidenden Tiles
 * avoidY - y-Koordinate des zu vermeidenden Tiles
 */
void generateMines(Tile *gameBoard, int64_t width, int64_t height, int mineCount, int64_t avoidX, int64_t avoidY)
{
    srand((unsigned int)time(NULL));
    int randomnumber;
    for (int i = 0; i < mineCount; i++)
    {
        do
        {
            randomnumber = (int)rand() % ((width * height) - 9);
            if (randomnumber >= (((avoidY - 1) * width) + (avoidX - 1)))
                randomnumber += 3;
            if (randomnumber >= ((avoidY * width) + (avoidX - 1)))
                randomnumber += 3;
            if (randomnumber >= (((avoidY + 1) * width) + (avoidX - 1)))
                randomnumber += 3;
        } while (gameBoard[randomnumber] & MINE_MASK);
        gameBoard[randomnumber] = MINE_MASK;
    }
}

unsigned int neighbouringMines(Tile *gameBoard, size_t width, size_t height, size_t tileX, size_t tileY)
{
    // wenn tilex tiley keine mine ist
    int mines = 0;
    size_t xstart, xend, ystart, yend;

    if (tileX == 0)
    {
        xstart = tileX;
        xend = tileX + 1;
    }
    else if (tileX == width - 1)
    {
        xend = tileX;
        xstart = tileX - 1;
    }
    else
    {
        xstart = tileX - 1;
        xend = tileX + 1;
    }

    if (tileY == 0)
    {
        ystart = tileY;
        yend = tileY + 1;
    }
    else if (tileY == height - 1)
    {
        yend = tileY;
        ystart = tileY - 1;
    }
    else
    {
        ystart = tileY - 1;
        yend = tileY + 1;
    }

    for (int i = xstart; i <= xend; i++)
    {
        for (int j = ystart; j <= yend; j++)
        {
            if (gameBoard[j * width + i] & MINE_MASK)
                mines++;
        }
    }

    return mines;
}

int64_t clamp(int64_t min, int64_t max, int64_t i)
{
    return i < min ? min : (i > max ? max : i);
}

char revealTile(Tile *gameBoard, size_t width, size_t height, size_t x, size_t y)
{
    Tile *tile = &gameBoard[x + width * y];
    if (*tile & REVEALED_MASK || *tile & MARKED_MASK)
        return 0;

    *tile = *tile | REVEALED_MASK;
    revealedTiles++;

    if (*tile & MINE_MASK)
        return 1;

    unsigned int count = neighbouringMines(gameBoard, width, height, x, y);
    *tile = *tile | ((count << 2) & SYMBOL_MASK); // "& SYMBOL_MASK" only for safety

    if (count == 0)
    {
        int minX = clamp(0, width - 1, x - 1);
        int maxX = clamp(0, width - 1, x + 1);
        int minY = clamp(0, height - 1, y - 1);
        int maxY = clamp(0, height - 1, y + 1);

        for (size_t i = minX; i <= maxX; i++)
            for (size_t j = minY; j <= maxY; j++)
                revealTile(gameBoard, width, height, i, j);
    }
    return 0;
}

void calcGameDimensions(GameDimensions *dimensions, int windowWidth, int windowHeight, int boardTileWidth, int boardTileHeight)
{
    SDL_Rect boardArea = {
        BORDER_SIZE,
        2 * BORDER_SIZE + TOPBAR_HEIGHT,
        windowWidth - 2 * BORDER_SIZE,
        windowHeight - 3 * BORDER_SIZE - TOPBAR_HEIGHT,
    };

    int vWidth = boardArea.h * ((float)(boardTileWidth) / (float)(boardTileHeight));
    int hHeight = boardArea.w * ((float)(boardTileHeight) / (float)(boardTileWidth));

    if (hHeight < boardArea.h)
    {
        dimensions->boardLeft = boardArea.x;
        dimensions->boardRight = boardArea.x + boardArea.w;
        int center = boardArea.y + boardArea.h / 2;
        dimensions->boardTop = center - hHeight / 2;
        dimensions->boardBottom = center + hHeight / 2;
    }
    else
    {
        dimensions->boardTop = boardArea.y;
        dimensions->boardBottom = boardArea.y + boardArea.h;
        int center = boardArea.x + boardArea.w / 2;
        dimensions->boardLeft = center - vWidth / 2;
        dimensions->boardRight = center + vWidth / 2;
    }
    dimensions->tileSize = ((float)(dimensions->boardRight - dimensions->boardLeft)) / boardTileWidth;
}
