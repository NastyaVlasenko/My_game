#pragma once
#include "entity.h"

enum class Character_type {
    KNIGHT
};

class Character: public Entity {
private:
    Character_type type;
    int max_hp;
    int hp;
    int base_damage;
    int damage;
    int level;
    int experience;
    bool friendly;
    
    void level_up();
    
public:
    Character (std::string name, Character_type type, int max_hp, int damage, bool friendly);

    char get_symbol() const override;
    std::string get_color() const override;
    
    // Combat API
    int get_hp() const { return hp; }
    int get_max_hp() const { return max_hp; }
    int get_damage() const { return damage; }
    void take_damage(int amount) { hp = std::max(0, hp - amount); }
    bool is_alive() const { return hp > 0; }
    bool is_friendly() const { return friendly; }
    
    // Level API
    int get_level() const { return level; }
    int get_experience() const { return experience; }
    void add_experience(int amount);
};
