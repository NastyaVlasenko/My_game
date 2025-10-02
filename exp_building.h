#pragma once
#include "building.h"

enum class Exp_building_type {
    SMALL_CAMP,
    LARGE_CAMP
};

class Exp_building: public Building {
private:
    Exp_building_type type;
    int exp_per_turn;
public:
    Exp_building (std::string name, Exp_building_type type, bool friendly);

    char get_symbol() const override;
    std::string get_color() const override;
    int get_exp_per_turn() const { return exp_per_turn; }
};
