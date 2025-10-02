#include "citadel.h"
#include <iostream>
#include <algorithm>

Citadel::Citadel (std::string name, Citadel_type type, int max_hp, bool friendly)
    : Entity(name), type(type), max_hp(max_hp), friendly(friendly) {
    hp = max_hp;
}

char Citadel::get_symbol() const {
    switch(type) {
        case Citadel_type::CITADEL: return 'X';
        default: return '#';
    }
}

std::string Citadel::get_color() const {
    return friendly ? "\033[38;5;27m" : "\033[38;5;160m";
}

void Citadel::take_damage(int damage) {
    hp = std::max(0, hp - damage);
    std::cout << "Citadel took " << damage << " damage! HP: " << hp << "/" << max_hp << std::endl;
    
    if (is_destroyed()) {
        std::cout << "Citadel has been destroyed!" << std::endl;
    }
}
