#include "ai_player.h"
#include "citadel.h"
#include "resource.h"
#include "chest.h"
#include "attack_building.h"
#include "gold_building.h"
#include "exp_building.h"
#include <iostream>
#include <cmath>
#include <random>
#include <climits>

AIPlayer::AIPlayer(Character* character) : Player(character), next_building_type(0) {
}

void AIPlayer::make_turn(Map* map) {
    if (!map || !character) return;
    
    std::cout << "\n=== AI TURN ===" << std::endl;
    
    // Check if player is close to AI citadel first
    int player_x, player_y, ai_citadel_x, ai_citadel_y;
    find_player_and_ai_citadel(map, player_x, player_y, ai_citadel_x, ai_citadel_y);
    
    bool player_near_citadel = false;
    if (player_x != -1 && player_y != -1 && ai_citadel_x != -1 && ai_citadel_y != -1) {
        int distance = abs(player_x - ai_citadel_x) + abs(player_y - ai_citadel_y);
        if (distance <= 5) {
            player_near_citadel = true;
        }
    }
    
    if (player_near_citadel) {
        // Only attack or move towards player
        if (try_attack_enemy(map)) {
            return;
        }
        
        if (player_x != -1 && player_y != -1) {
            if (try_move_towards_target(map, player_x, player_y)) {
                std::cout << "AI moves towards player!" << std::endl;
            }
        }
        return;
    }
    
    // Normal AI behavior when player is far from citadel
    // AI action priorities:
    // 1. Attack enemy if nearby
    // 2. Harvest resources if nearby
    // 3. Build buildings in order: gold field -> exp camp -> attack tower
    // 4. Upgrade nearby buildings
    // 5. Move towards nearest target (enemy or resource)
    
    if (try_attack_enemy(map)) {
        std::cout << "AI attacks enemy!" << std::endl;
        return;
    }
    
    if (try_harvest_resource(map)) {
        std::cout << "AI harvests resource!" << std::endl;
        return;
    }
    
    // Try to build buildings in cyclic order: gold -> exp -> attack -> gold -> ...
    bool built_something = false;
    for (int attempt = 0; attempt < 3; attempt++) {
        if (next_building_type == 0) {
            if (try_build_gold_structure(map)) {
                std::cout << "AI builds gold building!" << std::endl;
                built_something = true;
                break;
            }
        } else if (next_building_type == 1) {
            if (try_build_exp_structure(map)) {
                std::cout << "AI builds experience building!" << std::endl;
                built_something = true;
                break;
            }
        } else if (next_building_type == 2) {
            if (try_build_attack_tower(map)) {
                std::cout << "AI builds attack tower!" << std::endl;
                built_something = true;
                break;
            }
        }
        
        // Move to next building type
        next_building_type = (next_building_type + 1) % 3;
    }
    
    if (built_something) {
        // Move to next building type for next time
        next_building_type = (next_building_type + 1) % 3;
        return;
    }
    
    // Try to upgrade nearby buildings
    if (try_upgrade_nearby_buildings(map)) {
        std::cout << "AI upgrades building!" << std::endl;
        return;
    }
    
    // Move towards nearest target (enemy or resource)
    int enemy_x, enemy_y, resource_x, resource_y;
    find_nearest_enemy(map, enemy_x, enemy_y);
    find_nearest_resource(map, resource_x, resource_y);
    
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    
    int target_x = -1, target_y = -1;
    int min_distance = INT_MAX;
    
    // Compare distances to enemy and resource
    if (enemy_x != -1 && enemy_y != -1) {
        int enemy_distance = abs(enemy_x - char_x) + abs(enemy_y - char_y);
        if (enemy_distance < min_distance) {
            min_distance = enemy_distance;
            target_x = enemy_x;
            target_y = enemy_y;
        }
    }
    
    if (resource_x != -1 && resource_y != -1) {
        int resource_distance = abs(resource_x - char_x) + abs(resource_y - char_y);
        if (resource_distance < min_distance) {
            min_distance = resource_distance;
            target_x = resource_x;
            target_y = resource_y;
        }
    }
    
    if (target_x != -1 && target_y != -1) {
        if (try_move_towards_target(map, target_x, target_y)) {
            std::cout << "AI moves towards target!" << std::endl;
        }
    }
}

void AIPlayer::find_character_position(Map* map, int& x, int& y) {
    x = -1;
    y = -1;
    
    for (int py = 0; py < map->get_height(); py++) {
        for (int px = 0; px < map->get_width(); px++) {
            Cell* cell = map->get_cell(px, py);
            if (cell && cell->get_entity() == character) {
                x = px;
                y = py;
                return;
            }
        }
    }
}

void AIPlayer::find_player_and_ai_citadel(Map* map, int& player_x, int& player_y, int& ai_citadel_x, int& ai_citadel_y) {
    player_x = -1;
    player_y = -1;
    ai_citadel_x = -1;
    ai_citadel_y = -1;
    
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Character* character = dynamic_cast<Character*>(entity);
                Citadel* citadel = dynamic_cast<Citadel*>(entity);
                
                if (character && character != this->character) {
                    // This is the player character
                    player_x = x;
                    player_y = y;
                }
                
                if (citadel && !citadel->is_friendly()) {
                    // This is the AI citadel
                    ai_citadel_x = x;
                    ai_citadel_y = y;
                }
            }
        }
    }
}

void AIPlayer::find_nearest_enemy(Map* map, int& target_x, int& target_y) {
    target_x = -1;
    target_y = -1;
    
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) {
        return;
    }
    
    int min_distance = INT_MAX;
    int player_distance = INT_MAX;
    int player_x = -1, player_y = -1;
    int enemy_citadel_x = -1, enemy_citadel_y = -1;
    
    // First pass: find player character and enemy citadel
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Character* enemy_char = dynamic_cast<Character*>(entity);
                Citadel* enemy_citadel = dynamic_cast<Citadel*>(entity);
                
                if (enemy_char && enemy_char != character) {
                    int distance = abs(x - char_x) + abs(y - char_y);
                    if (distance < player_distance) {
                        player_distance = distance;
                        player_x = x;
                        player_y = y;
                    }
                }
                
                if (enemy_citadel && !enemy_citadel->is_friendly()) {
                    enemy_citadel_x = x;
                    enemy_citadel_y = y;
                }
            }
        }
    }
    
    // Check if player is within 5 cells of AI citadel
    if (player_x != -1 && player_y != -1 && enemy_citadel_x != -1 && enemy_citadel_y != -1) {
        int player_to_citadel_distance = abs(player_x - enemy_citadel_x) + abs(player_y - enemy_citadel_y);
        
        if (player_to_citadel_distance <= 5) {
            target_x = player_x;
            target_y = player_y;
            return;
        }
    }
    
    // Normal target selection: choose closest enemy
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Character* enemy_char = dynamic_cast<Character*>(entity);
                Citadel* enemy_citadel = dynamic_cast<Citadel*>(entity);
                
                if ((enemy_char && enemy_char != character) || 
                    (enemy_citadel && enemy_citadel->is_friendly())) {
                    int distance = abs(x - char_x) + abs(y - char_y);
                    if (distance < min_distance) {
                        min_distance = distance;
                        target_x = x;
                        target_y = y;
                    }
                }
            }
        }
    }
}

void AIPlayer::find_nearest_resource(Map* map, int& target_x, int& target_y) {
    target_x = -1;
    target_y = -1;
    
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) {
        return;
    }
    
    int min_distance = INT_MAX;
    
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Resource* resource = dynamic_cast<Resource*>(entity);
                Chest* chest = dynamic_cast<Chest*>(entity);
                
                if (resource || chest) {
                    int distance = abs(x - char_x) + abs(y - char_y);
                    if (distance < min_distance) {
                        min_distance = distance;
                        target_x = x;
                        target_y = y;
                    }
                }
            }
        }
    }
}


void AIPlayer::find_best_build_position(Map* map, int& target_x, int& target_y) {
    target_x = -1;
    target_y = -1;
    
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) {
        return;
    }
    
    // Ищем пустую клетку в 4 соседних клетках (WASD)
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // W, S, A, D
    
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int x = char_x + dx;
        int y = char_y + dy;
        
        if (x >= 0 && y >= 0 && x < map->get_width() && y < map->get_height()) {
            Cell* cell = map->get_cell(x, y);
            if (cell && cell->is_empty()) {
                target_x = x;
                target_y = y;
                return;
            }
        }
    }
}

bool AIPlayer::try_attack_enemy(Map* map) {
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) return false;
    
    // Check 4-neighbors only
    const int dirs[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};
    for (auto& d : dirs) {
        int x = char_x + d[0];
        int y = char_y + d[1];
        if (x >= 0 && y >= 0 && x < map->get_width() && y < map->get_height()) {
            if (attack_entity(map, x, y)) return true;
        }
    }
    
    return false;
}

bool AIPlayer::try_harvest_resource(Map* map) {
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) return false;
    
    // Check 4-neighbors only
    const int dirs2[4][2] = {{0,-1},{0,1},{-1,0},{1,0}};
    for (auto& d : dirs2) {
        int x = char_x + d[0];
        int y = char_y + d[1];
        if (x >= 0 && y >= 0 && x < map->get_width() && y < map->get_height()) {
            if (harvest_resource(map, x, y)) return true;
        }
    }
    
    return false;
}

bool AIPlayer::try_build_exp_structure(Map* map) {
    // Check if we already have an exp building
    if (has_building_type(map, "exp_building")) {
        return false;
    }
    
    if (gold < 75 || wood < 1 || stone < 1) {
        return false;
    }
    
    int build_x, build_y;
    find_best_build_position(map, build_x, build_y);
    
    if (build_x == -1 || build_y == -1) {
        return false;
    }
    
    return build_structure(map, build_x, build_y, "exp_building");
}

bool AIPlayer::try_build_gold_structure(Map* map) {
    // Check if we already have a gold building
    if (has_building_type(map, "gold_building")) {
        return false;
    }
    
    if (gold < 50 || wood < 2) {
        return false;
    }
    
    int build_x, build_y;
    find_best_build_position(map, build_x, build_y);
    
    if (build_x == -1 || build_y == -1) {
        return false;
    }
    
    return build_structure(map, build_x, build_y, "gold_building");
}

bool AIPlayer::try_build_attack_tower(Map* map) {
    // Check if we already have an attack tower
    if (has_building_type(map, "attack_building")) {
        return false;
    }
    
    if (gold < 50 || stone < 2) {
        return false;
    }
    
    int build_x, build_y;
    find_best_build_position(map, build_x, build_y);
    
    if (build_x == -1 || build_y == -1) {
        return false;
    }
    
    return build_structure(map, build_x, build_y, "attack_building");
}

bool AIPlayer::try_upgrade_nearby_buildings(Map* map) {
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) return false;
    
    // Check 4 adjacent cells for buildings to upgrade
    int directions[4][2] = {{0, -1}, {0, 1}, {-1, 0}, {1, 0}}; // W, S, A, D
    
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        int x = char_x + dx;
        int y = char_y + dy;
        
        if (x >= 0 && y >= 0 && x < map->get_width() && y < map->get_height()) {
            if (upgrade_building(map, dx, dy)) {
                return true;
            }
        }
    }
    
    return false;
}

bool AIPlayer::has_building_type(Map* map, const std::string& building_type) {
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Building* building = dynamic_cast<Building*>(entity);
                
                if (building && building->is_friendly() == is_friendly_side()) {
                    // Check building type
                    if (building_type == "gold_building") {
                        Gold_building* gold_building = dynamic_cast<Gold_building*>(entity);
                        if (gold_building) return true;
                    } else if (building_type == "exp_building") {
                        Exp_building* exp_building = dynamic_cast<Exp_building*>(entity);
                        if (exp_building) return true;
                    } else if (building_type == "attack_building") {
                        Attack_building* attack_building = dynamic_cast<Attack_building*>(entity);
                        if (attack_building) return true;
                    }
                }
            }
        }
    }
    return false;
}

bool AIPlayer::try_move_towards_target(Map* map, int target_x, int target_y) {
    int char_x, char_y;
    find_character_position(map, char_x, char_y);
    if (char_x == -1 || char_y == -1) return false;
    
    // Try to move towards target, but if blocked, try alternative directions
    int directions[4][2] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}}; // up, down, right, left
    
    // First, try the best direction towards target
    int best_dx = 0, best_dy = 0;
    if (std::abs(target_x - char_x) >= std::abs(target_y - char_y)) {
        if (target_x > char_x) best_dx = 1; else if (target_x < char_x) best_dx = -1;
    } else {
        if (target_y > char_y) best_dy = 1; else if (target_y < char_y) best_dy = -1;
    }
    
    // Try best direction first
    int new_x = char_x + best_dx;
    int new_y = char_y + best_dy;
    if (move_character(map, new_x, new_y)) {
        return true;
    }
    
    // If blocked, try other directions in order of preference
    for (int i = 0; i < 4; i++) {
        int dx = directions[i][0];
        int dy = directions[i][1];
        
        // Skip if this is the direction we already tried
        if (dx == best_dx && dy == best_dy) continue;
        
        new_x = char_x + dx;
        new_y = char_y + dy;
        
        if (move_character(map, new_x, new_y)) {
            return true;
        }
    }
    
    return false; // All directions blocked
}
