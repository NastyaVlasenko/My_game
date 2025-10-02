#include "building.h"

Building::Building(bool friendly, std::string name) 
    : Entity(name), friendly(friendly) {}

bool Building::is_friendly() const {
    return friendly;
}

char Building::get_symbol() const {
    return 'B'; // Символ для зданий
}

std::string Building::get_color() const {
    return friendly ? "\033[38;5;34m" : "\033[38;5;196m"; // Зеленый для дружественных, красный для вражеских
}
