#pragma once
#include <vector>
#include <string>
#include "level.h"

class LevelManager {
private:
    std::vector<Level> levels;
    int current_level;
    bool all_levels_completed;
    
public:
    LevelManager();
    ~LevelManager() = default;
    
    void initialize_levels();
    bool has_next_level() const;
    bool is_all_completed() const;
    Level& get_current_level();
    void complete_current_level();
    void display_level_info() const;
    void display_victory_message() const;
    void display_defeat_message() const;
    void display_all_completed_message() const;
};
