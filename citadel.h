#pragma once
#include "entity.h"

enum class Citadel_type {
    CITADEL
};

class Citadel: public Entity {
private:
    Citadel_type type;
    int max_hp;
    int hp;
    bool friendly;
public:
    Citadel (std::string name, Citadel_type type, int max_hp, bool friendly);

    char get_symbol() const override;
    std::string get_color() const override;
    bool is_friendly() const { return friendly; }
    
    // HP management
    int get_hp() const { return hp; }
    int get_max_hp() const { return max_hp; }
    bool is_destroyed() const { return hp <= 0; }
    void take_damage(int damage);
};

