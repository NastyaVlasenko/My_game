#pragma once
#include "entity.h"

enum class Chest_type {
    COMMON_CHEST,
    RARE_CHEST,
    EPIC_CHEST
};

class Chest: public Entity {
private:
    Chest_type type;
    int gold_amount;
    int wood_amount;
    int stone_amount;
public:
    Chest (std::string name, Chest_type type);

    char get_symbol() const override;
    std::string get_color() const override;
    int get_gold_amount() const { return gold_amount; }
    int get_wood_amount() const { return wood_amount; }
    int get_stone_amount() const { return stone_amount; }
};
