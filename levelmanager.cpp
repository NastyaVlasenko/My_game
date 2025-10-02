#include "levelmanager.h"
#include <iostream>

LevelManager::LevelManager() : current_level(0), all_levels_completed(false) {
    initialize_levels();
}

void LevelManager::initialize_levels() {
    levels.clear();
    levels.push_back(Level(1, "Plains", "Level1"));
    levels.push_back(Level(2, "Desert", "Level2"));
    levels.push_back(Level(3, "Taiga", "Level3"));
}

bool LevelManager::has_next_level() const {
    return current_level < static_cast<int>(levels.size()) - 1;
}

bool LevelManager::is_all_completed() const {
    return all_levels_completed;
}

Level& LevelManager::get_current_level() {
    return levels[current_level];
}

void LevelManager::complete_current_level() {
    levels[current_level].set_completed(true);
    
    if (has_next_level()) {
        current_level++;
    } else {
        all_levels_completed = true;
    }
}

void LevelManager::display_level_info() const {
    const Level& level = levels[current_level];
    std::cout << "\n=== LEVEL " << level.get_number() << ": " << level.get_name() << " ===" << std::endl;
    std::cout << "Objective: Defeat the enemy!" << std::endl;
}

void LevelManager::display_victory_message() const {
    // Show victory for the current level (before it gets incremented)
    const Level& level = levels[current_level];
    std::cout << "\n VICTORY! " << std::endl;
    std::cout << "You have conquered " << level.get_name() << "!" << std::endl;
    
    if (has_next_level()) {
        std::cout << "\n=== LEVEL " << current_level + 2 << " UNLOCKED ===" << std::endl;
        std::cout << "Moving to " << levels[current_level + 1].get_name() << "!" << std::endl;
        std::cout << "Press Enter to continue to the next level..." << std::endl;
        std::cin.ignore();
        std::cin.get();
    } else {
        display_all_completed_message();
    }
}

void LevelManager::display_defeat_message() const {
    const Level& level = levels[current_level];
    std::cout << "\n DEFEAT! " << std::endl;
    std::cout << "You were defeated in " << level.get_name() << "!" << std::endl;
    std::cout << "Press Enter to try again..." << std::endl;
    std::cin.ignore();
    std::cin.get();
}

void LevelManager::display_all_completed_message() const {
    std::cout << "\n CONGRATULATIONS! " << std::endl;
    std::cout << "You have completed ALL levels!" << std::endl;
    std::cout << "Thank you for playing!" << std::endl;
    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.ignore();
    std::cin.get();
}
