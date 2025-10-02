#pragma once
#include "entity.h"

class Cell{
private:
    int x;
    int y;
    Entity *entity;

public:
    Cell(int x, int y);

    bool is_empty();

    Entity* get_entity();
    void set_entity(Entity* new_entity);

    int get_x() const { return x; }
    int get_y() const { return y; }
};
