#include "game.h"
#include "character.h"
#include "citadel.h"
#include "maploader.h"
#include "render.h"
#include <iostream>
#include <chrono>
#include <thread>

Game::Game() : map(nullptr), is_over(false), human_turn(true), turn_count(0), game_won(false) {
}

Game::~Game() {
    delete map;
}

void Game::initialize() {
    // Display current level info
    level_manager.display_level_info();
    
    // Load map from current level file
    Level& current_level = level_manager.get_current_level();
    map = Maploader::terrain_from_file(current_level.get_filename());
    if (!map) {
        std::cerr << "Error loading map!" << std::endl;
        return;
    }
    
    // Load entities onto map (including characters and chests)
    Maploader::entities_from_file(map, current_level.get_filename());
    
    // Find characters from the map and create players
    Character* human_char = nullptr;
    Character* ai_char = nullptr;
    
    
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Character* character = dynamic_cast<Character*>(entity);
                if (character) {
                    if (character->is_friendly()) {
                        human_char = character;
                    } else {
                        ai_char = character;
                    }
                }
            }
        }
    }
    
    if (!human_char || !ai_char) {
        std::cerr << "Failed to find characters on the map!" << std::endl;
        is_over = true;
        return;
    }
    
    // Create players with found characters
    human_player = std::make_unique<HumanPlayer>(human_char);
    ai_player = std::make_unique<AIPlayer>(ai_char);
    std::cout << "Objective: Destroy the enemy citadel!" << std::endl;
}

void Game::start() {
    std::cout << "\n=== TOWER DEFENSE GAME ===" << std::endl;
    
    while (!level_manager.is_all_completed()) {
        // Initialize current level
        initialize();
        if (is_over) return; // Error loading level
        
        // Play current level
        while (!is_over) {
            next_turn();
            check_game_over();
            
            if (!is_over) {
                // Small pause between turns
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            }
        }
        
        // Handle level completion (without reinitializing)
        end_game();
    }
}

void Game::next_turn() {
    turn_count++;
    
    Player* current_player = get_current_player();
    if (!current_player) return;
    
    std::cout << "\n=== TURN " << turn_count << " ===" << std::endl;
    std::cout << "Current player: " << (human_turn ? "Player" : "AI") << std::endl;
    // Show both characters' HP
    Character* hc=nullptr; Character* ac=nullptr;
    for (int y = 0; y < map->get_height(); ++y) {
        for (int x = 0; x < map->get_width(); ++x) {
            Cell* c = map->get_cell(x,y);
            if (!c || c->is_empty()) continue;
            Character* ch = dynamic_cast<Character*>(c->get_entity());
            if (ch) {
                if (ch->is_friendly()) { hc = ch; }
                else { ac = ch; }
            }
        }
    }
    if (hc) std::cout << "Player HP: " << hc->get_hp() << "/" << hc->get_max_hp() 
                      << " | Level: " << hc->get_level() << " | Damage: " << hc->get_damage() << std::endl;
    if (ac) std::cout << "AI HP: " << ac->get_hp() << "/" << ac->get_max_hp() 
                      << " | Level: " << ac->get_level() << " | Damage: " << ac->get_damage() << std::endl;
    
    // Show citadel HP
    for (int y = 0; y < map->get_height(); ++y) {
        for (int x = 0; x < map->get_width(); ++x) {
            Cell* c = map->get_cell(x,y);
            if (!c || c->is_empty()) continue;
            Citadel* citadel = dynamic_cast<Citadel*>(c->get_entity());
            if (citadel) {
                std::cout << (citadel->is_friendly() ? "Player" : "AI") << " Citadel HP: " 
                          << citadel->get_hp() << "/" << citadel->get_max_hp() << std::endl;
            }
        }
    }
    
    // Start current player's turn
    current_player->start_turn();
    
    // Collect resources from buildings
    current_player->collect_resources_from_buildings(map);
    
    // Process tower attacks
    current_player->process_tower_attacks(map);
    
    // Execute turn
    current_player->make_turn(map);
    
    // End turn
    current_player->end_turn();
    
    // Switch turns
    human_turn = !human_turn;
}

void Game::check_game_over() {
    if (!map) {
        is_over = true;
        return;
    }
    
    // Check characters alive status
    bool human_alive = false;
    bool ai_alive = false;
    for (int y = 0; y < map->get_height(); ++y) {
        for (int x = 0; x < map->get_width(); ++x) {
            Cell* c = map->get_cell(x,y);
            if (!c || c->is_empty()) continue;
            Character* ch = dynamic_cast<Character*>(c->get_entity());
            if (ch) {
                if (ch->is_friendly() && ch->is_alive()) human_alive = true;
                if (!ch->is_friendly() && ch->is_alive()) ai_alive = true;
            }
        }
    }
    if (!human_alive) {
        game_won = false;
        is_over = true;
        return;
    }
    if (!ai_alive) {
        game_won = true;
        is_over = true;
        return;
    }
    
    // Check if human player won (citadel destroyed)
    if (human_player && human_player->has_won(map)) {
        std::cout << "\n🎉 CONGRATULATIONS! You won!" << std::endl;
        is_over = true;
        return;
    }
    
    // Check if AI won
    if (ai_player && ai_player->has_won(map)) {
        std::cout << "\n💀 AI won! Try again!" << std::endl;
        is_over = true;
        return;
    }
}

void Game::render() {
    if (map) {
        Render::print_map(map);
    }
}

void Game::end_game() {
    if (game_won) {
        level_manager.display_victory_message();
        level_manager.complete_current_level();
        
        if (level_manager.is_all_completed()) {
            return; // Game completely finished
        }
    } else {
        level_manager.display_defeat_message();
    }
    
    // Reset game state for next level
    is_over = false;
    human_turn = true;
    turn_count = 0;
    game_won = false;
    
    // Clean up current map
    delete map;
    map = nullptr;
    
    // Level will be automatically loaded in the next iteration of start() loop
}


Player* Game::get_current_player() {
    return human_turn ? human_player.get() : ai_player.get();
}
