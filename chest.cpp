#include "chest.h"

Chest::Chest (std::string name, Chest_type type)
    : Entity(name), type(type) {
    if (type == Chest_type::COMMON_CHEST) {
        gold_amount = 20;
        wood_amount = 1;
        stone_amount = 1;
    } else if (type == Chest_type::RARE_CHEST) {
        gold_amount = 50;
        wood_amount = 2;
        stone_amount = 2;
    } else if (type == Chest_type::EPIC_CHEST) {
        gold_amount = 100;
        wood_amount = 5;
        stone_amount = 3;
    }
}

char Chest::get_symbol() const {return '?';}

std::string Chest::get_color() const {
    switch(type) {
        case Chest_type::COMMON_CHEST: return "\033[38;5;94m";  // Yellow
        case Chest_type::RARE_CHEST: return "\033[38;5;220m";    // Cyan
        case Chest_type::EPIC_CHEST: return "\033[38;5;13m";    // Magenta
        default: return "";
    }
}
