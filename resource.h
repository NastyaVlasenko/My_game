#pragma once
#include "entity.h"
enum class Resource_type {
    TREE,
    ROCK
};

class Resource: public Entity {
private:
    Resource_type type;
public:
    Resource (std::string name, Resource_type type);

    char get_symbol() const override;
    std::string get_color() const override;
    Resource_type get_type() const { return type; }
};
