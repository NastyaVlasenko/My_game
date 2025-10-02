#include "attack_building.h"

Attack_building::Attack_building(std::string name, Attack_building_type type, bool friendly)
    : Building(friendly, name), type(type) {
    if (type == Attack_building_type::SMALL_TOWER) {
        damage = 1;
    } else if (type == Attack_building_type::LARGE_TOWER) {
        damage = 2;
    }
}

char Attack_building::get_symbol() const {
    switch(type) {
        case Attack_building_type::SMALL_TOWER: return 'y';
        case Attack_building_type::LARGE_TOWER: return 'Y';
        default: return 'T';
    }
}

std::string Attack_building::get_color() const {
    return is_friendly() ? "\033[38;5;27m" : "\033[38;5;160m"; // Green for friendly, red for enemy
}
