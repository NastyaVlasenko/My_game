#pragma once
#include "building.h"

enum class Gold_building_type {
    SMALL_FIELD,
    LARGE_FIELD
};

class Gold_building: public Building {
private:
    Gold_building_type type;
    int gold_per_turn;
public:
    Gold_building (std::string name, Gold_building_type type, bool friendly);

    char get_symbol() const override;
    std::string get_color() const override;
    int get_gold_per_turn() const { return gold_per_turn; }
};
