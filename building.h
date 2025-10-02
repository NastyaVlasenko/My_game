#pragma once
#include "entity.h"

class Building: public Entity {
private:
    bool friendly;
public:
    Building(bool friendly, std::string name);

    bool is_friendly() const;
    
    // Реализация чисто виртуальных методов из Entity
    char get_symbol() const override;
    std::string get_color() const override;
};
