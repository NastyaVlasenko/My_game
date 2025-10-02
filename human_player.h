#pragma once
#include "player.h"

class HumanPlayer : public Player {
public:
    HumanPlayer(Character* character);
    
    void make_turn(Map* map) override;
    
private:
    void display_menu();
    void display_map(Map* map);
    void display_resources();
    int get_user_choice();
    void handle_movement(Map* map);
    void handle_harvest(Map* map);
    void handle_build(Map* map);
    void handle_attack(Map* map);
    void handle_upgrade(Map* map);
};
