#pragma once

#include "map.h"
#include "player.h"
#include "human_player.h"
#include "ai_player.h"
#include "levelmanager.h"
#include <memory>

class Game {
private:
    Map* map;
    std::unique_ptr<Player> human_player;
    std::unique_ptr<Player> ai_player;
    LevelManager level_manager;
    bool is_over;
    bool human_turn;
    int turn_count;
    bool game_won;
    
public:
    Game();
    ~Game();
    void initialize();
    void start();
    void next_turn();
    void check_game_over();
    void render();
    void end_game();
    bool is_game_over() const { return is_over; }
    Player* get_current_player();
};
