#include "cell.h"
#include <iostream>

Cell::Cell(int x, int y):
    x(x), y(y), entity(nullptr) {
        if (x < 0 || y < 0)
        {
            std::cerr << "Warning: cell created with negative coordinates" << std::endl;
        }
}

bool Cell::is_empty(){
    return (entity == nullptr);
}

Entity* Cell::get_entity() {return entity;}
void Cell::set_entity(Entity* new_entity) {entity = new_entity;}
