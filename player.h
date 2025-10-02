#pragma once
#include "character.h"
#include "map.h"
#include <vector>

class Player {
protected:
    Character* character;
    int gold;
    int wood;
    int stone;
    bool is_turn;
    
    // Helper to validate 4-neighbor adjacency
    bool is_adjacent_four(int from_x, int from_y, int to_x, int to_y) const;
    bool find_character_position(Map* map, int& out_x, int& out_y) const;
    
public:
    Player(Character* character);
    virtual ~Player() = default;
    
    // Side allegiance: true for human (friendly/blue), false for AI (hostile/red)
    virtual bool is_friendly_side() const { return true; }
    
    // Основные методы
    virtual void make_turn(Map* map) = 0; // Чисто виртуальная функция
    void end_turn();
    void start_turn();
    
    // Действия игрока
    bool move_character(Map* map, int new_x, int new_y);
    bool harvest_resource(Map* map, int x, int y);
    bool build_structure(Map* map, int x, int y, std::string building_type);
    bool attack_entity(Map* map, int x, int y);
    
    // Получение ресурсов
    void add_gold(int amount);
    void add_experience(int amount);
    void add_wood(int amount);
    void add_stone(int amount);
    void collect_resources_from_buildings(Map* map);
    
    
    // Tower attacks
    void process_tower_attacks(Map* map);
    
    // Building upgrade
    bool upgrade_building(Map* map, int dx, int dy);
    
    // Геттеры
    Character* get_character() const { return character; }
    int get_gold() const { return gold; }
    int get_experience() const { return character->get_experience(); }
    int get_wood() const { return wood; }
    int get_stone() const { return stone; }
    bool is_my_turn() const { return is_turn; }
    
    // Проверка победы
    bool has_won(Map* map);
};
