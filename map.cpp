#include "map.h"

Map::Map(int width, int height, Biome* biome): width(width), height(height), biome(biome) {
        for (int y = 0; y < height; ++y)
        {
            std::vector<Cell> row;
            for (int x = 0; x < width; ++x)
            {
                row.push_back(Cell(x, y));
            }
            cells.push_back(row);
        }
}

Map::~Map() {
    if (biome) {
        delete biome;
    }
}

Cell* Map::get_cell(int x, int y) {
    if (x >= 0 && y >= 0 && x < width && y < height) {return &cells[y][x];}
    else {return nullptr;}
}

int Map::get_width() const {return width;}
int Map::get_height() const {return height;}
