#pragma once
#include "player.h"

class AIPlayer : public Player {
public:
    AIPlayer(Character* character);
    
    void make_turn(Map* map) override;
    bool is_friendly_side() const override { return false; }
    
private:
    int next_building_type; // 0=gold, 1=exp, 2=attack, cycles through 0-2
    void find_character_position(Map* map, int& x, int& y);
    void find_player_and_ai_citadel(Map* map, int& player_x, int& player_y, int& ai_citadel_x, int& ai_citadel_y);
    void find_nearest_enemy(Map* map, int& target_x, int& target_y);
    void find_nearest_resource(Map* map, int& target_x, int& target_y);
    void find_best_build_position(Map* map, int& target_x, int& target_y);
    bool try_attack_enemy(Map* map);
    bool try_harvest_resource(Map* map);
    bool try_build_exp_structure(Map* map);
    bool try_build_gold_structure(Map* map);
    bool try_build_attack_tower(Map* map);
    bool try_upgrade_nearby_buildings(Map* map);
    bool has_building_type(Map* map, const std::string& building_type);
    bool try_move_towards_target(Map* map, int target_x, int target_y);
};
