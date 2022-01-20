#include <stdint.h>
#include <stdlib.h>

// | Mine | Symbol            | Aufgedeckt | markiert |
// | 0    | 0 | 0 | 0 | 0 | 0 | 0          | 0        |

#define MINE_MASK     0b10000000
#define SYMBOL_MASK   0b01111100
#define REVEALED_MASK 0b00000010
#define MARKED_MASK   0b00000001

#define TOPBAR_CONTENT_HEIGHT 60
#define TOPBAR_PADDING 10
#define TOPBAR_HEIGHT (TOPBAR_CONTENT_HEIGHT + 2 * TOPBAR_PADDING)
#define BORDER_SIZE 13

#define EVENT_GAME_TIMER 1

typedef char Tile;

typedef struct
{
    int boardLeft, boardTop, boardRight, boardBottom;
    float tileSize;
} GameDimensions;

void printBinary(uint64_t d);

int64_t clamp(int64_t min, int64_t max, int64_t i);

void generateMines(Tile *gameBoard, int64_t width, int64_t height, int mineCount, int64_t avoidX, int64_t avoidY);
unsigned int neighbouringMines(Tile *gameBoard, size_t width, size_t height, size_t tileX, size_t tileY);

/**
 * \returns 0 - when no mine was revealed;
 *          1 - when mine was revealed;
 */
char revealTile(Tile *gameBoard, size_t width, size_t height, size_t x, size_t y);

void calcGameDimensions(GameDimensions *dimensions, int windowWidth, int windowHeight, int boardTileWidth, int boardTileHeight);