#pragma once
#include "entity.h"

enum class Barrier_type {
    WATER,
    THORNS
};

class Barrier: public Entity {
private:
    Barrier_type type;
public:
    Barrier (std::string name, Barrier_type type);

    char get_symbol() const override;
    std::string get_color() const override;

};
