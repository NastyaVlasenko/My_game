#pragma once
#include <vector>
#include "cell.h"
#include "biome.h"

class Map{
private:
    int width;
    int height;
    std::vector<std::vector<Cell>> cells;
    Biome* biome;

public:
    Map(int width, int height, Biome* biome = nullptr);
    ~Map();
    Cell* get_cell(int x, int y);
    int get_width() const;
    int get_height() const;
    Biome* get_biome() const { return biome; }
    void set_biome(Biome* new_biome) { biome = new_biome; }
};
