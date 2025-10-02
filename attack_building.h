#pragma once
#include "building.h"
#include <string>

enum class Attack_building_type {
    SMALL_TOWER,
    LARGE_TOWER
};

class Attack_building: public Building {
private:
    Attack_building_type type;
    int damage;
    
public:
    Attack_building(std::string name, Attack_building_type type, bool friendly);
    
    char get_symbol() const override;
    std::string get_color() const override;
    
    int get_damage() const { return damage; }
};
