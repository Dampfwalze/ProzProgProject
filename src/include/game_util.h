#include <stdint.h>
#include <stdlib.h>

// | Mine | Symbol            | Aufgedeckt | markiert |
// | 0    | 0 | 0 | 0 | 0 | 0 | 0          | 0        |

#define MINE_MASK     0b10000000
#define SYMBOL_MASK   0b01111100
#define REVEALED_MASK 0b00000010
#define MARKED_MASK   0b00000001

typedef char Tile;

typedef struct
{
    int boardLeft, boardTop, boardRight, boardBottom;
    float tileSize;
} GameDimensions;

void generateMines(Tile *gameBoard, size_t width, size_t height, int mineCount, size_t avoidX, size_t avoidY);
unsigned int neighbouringMines(Tile *gameBoard, size_t width, size_t height, size_t tileX, size_t tileY);
char revealTile(Tile *gameBoard, size_t width, size_t x, size_t y);

void calcGameDimensions(GameDimensions *dimensions, int windowWidth, int windowHeight, int boardTileWidth, int boardTileHeight);