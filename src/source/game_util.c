#include "game_util.h"
#include <time.h>
#include <stdlib.h>

void generateMines(Tile *gameBoard, size_t width, size_t height, int mineCount, size_t avoidX, size_t avoidY)
{
    srand((unsigned int) time(NULL));
    int i, randomnumber;
    for (i = 1; i <= mineCount; i++) {
        do {
            randomnumber = (int) rand() % ((width * height) - 1);
            if (randomnumber >= ((avoidY * width) + avoidX)) {
                randomnumber ++;
            }
        } while (gameBoard[randomnumber] & MINE_MASK);
        gameBoard[randomnumber] = MINE_MASK;
    }
}

unsigned int neighbouringMines(Tile *gameBoard, size_t width, size_t height, size_t tileX, size_t tileY)
{   
    //wenn tilex tiley keine mine ist
    int i, j, mines = 0;
    size_t xstart, xend, ystart, yend;

    if (tileX == 0) {
        xstart = tileX;
        xend = tileX + 1;
    } else 
    if (tileX == width - 1) {
        xend = tileX;
        xstart = tileX - 1;
    } else {
        xstart = tileX - 1;
        xend = tileX + 1;
    }

    if (tileY == 0) {
        ystart = tileY;
        yend = tileY + 1;
    } else 
    if (tileY == height - 1) {
        yend = tileY;
        ystart = tileY - 1;
    } else {
        ystart = tileY - 1;
        yend = tileY + 1;
    }

    for (i = xstart; i <= xend; i++) {
        for (j = ystart; j <= yend; j++) {
            if (gameBoard[tileY * width + tileX] & MINE_MASK)
                mines++;
        }
    }

    return mines;
}

char revealTile(Tile *gameBoard, size_t width, size_t x, size_t y)
{
    return (gameBoard[y * width + x] + 1);
}
