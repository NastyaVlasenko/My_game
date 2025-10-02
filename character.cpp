#include "character.h"
#include <iostream>

Character::Character (std::string name, Character_type type, int max_hp, int damage, bool friendly)
    :Entity(name), type(type), max_hp(max_hp), base_damage(damage), damage(damage), level(1), experience(0), friendly(friendly) {
    hp = max_hp;
}

char Character::get_symbol() const {
    switch(type) {
        case Character_type::KNIGHT: return '@';
        default: return '#';
    }
}

std::string Character::get_color() const {
    return friendly ? "\033[38;5;27m" : "\033[38;5;160m";
}

void Character::add_experience(int amount) {
    experience += amount;
    if (experience < 0) experience = 0;
    std::cout << "Character gained " << amount << " experience! Total: " << experience << std::endl;
    
    // Check for level up (every 1000 experience)
    while (experience >= 1000) {
        std::cout << "Character has " << experience << " experience, leveling up!" << std::endl;
        experience -= 1000;
        level_up();
        std::cout << "Character now has " << experience << " experience remaining" << std::endl;
    }
}

void Character::level_up() {
    level++;
    damage = base_damage + level - 1; // Damage increases by 1 per level
    std::cout << "Character leveled up to level " << level << "! Damage increased to " << damage << std::endl;
}
