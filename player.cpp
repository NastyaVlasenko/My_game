#include "player.h"
#include "resource.h"
#include "gold_building.h"
#include "exp_building.h"
#include "attack_building.h"
#include "citadel.h"
#include "chest.h"
#include "cell.h"
#include <iostream>

bool Player::is_adjacent_four(int from_x, int from_y, int to_x, int to_y) const {
    int dx = std::abs(to_x - from_x);
    int dy = std::abs(to_y - from_y);
    return (dx + dy) == 1; // Manhattan distance 1
}

bool Player::find_character_position(Map* map, int& out_x, int& out_y) const {
    if (!map || !character) return false;
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && cell->get_entity() == character) {
                out_x = x;
                out_y = y;
                return true;
            }
        }
    }
    return false;
}

Player::Player(Character* character) 
    : character(character), gold(100), wood(0), stone(0), is_turn(false) {
}

void Player::end_turn() {
    is_turn = false;
}

void Player::start_turn() {
    is_turn = true;
}

bool Player::move_character(Map* map, int new_x, int new_y) {
    if (!map || !character) return false;
    
    // Validate 4-neighbor move
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) return false;
    if (!is_adjacent_four(cur_x, cur_y, new_x, new_y)) return false;

    // Проверяем, что новая позиция в пределах карты
    if (new_x < 0 || new_y < 0 || new_x >= map->get_width() || new_y >= map->get_height()) {
        return false;
    }
    
    // Проверяем, что новая позиция пуста
    Cell* target_cell = map->get_cell(new_x, new_y);
    if (!target_cell || !target_cell->is_empty()) {
        return false;
    }
    
    // Move from current cell to target
    Cell* cur_cell = map->get_cell(cur_x, cur_y);
    if (!cur_cell) return false;
    cur_cell->set_entity(nullptr);
    target_cell->set_entity(character);
    return true;
}

bool Player::harvest_resource(Map* map, int x, int y) {
    if (!map) return false;
    // Must target 4-neighbor cell relative to character
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) return false;
    if (!is_adjacent_four(cur_x, cur_y, x, y)) return false;
    
    Cell* cell = map->get_cell(x, y);
    if (!cell || cell->is_empty()) return false;
    
    Entity* entity = cell->get_entity();
    Resource* resource = dynamic_cast<Resource*>(entity);
    
    if (resource) {
        // Add resources based on type through enum
        if (resource->get_type() == Resource_type::TREE) {
            add_wood(1); // Tree gives wood
            add_experience(30); // Tree gives 30 experience
        } else if (resource->get_type() == Resource_type::ROCK) {
            add_stone(1); // Rock gives stone
            add_experience(50); // Rock gives 50 experience
        }
        
        // Remove resource after harvesting
        cell->set_entity(nullptr);
        delete resource;
        return true;
    }
    
    // Check for chest
    Chest* chest = dynamic_cast<Chest*>(entity);
    if (chest) {
        // Add chest resources
        add_gold(chest->get_gold_amount());
        add_wood(chest->get_wood_amount());
        add_stone(chest->get_stone_amount());
        
        // Remove chest after harvesting
        cell->set_entity(nullptr);
        delete chest;
        return true;
    }
    
    return false;
}

bool Player::build_structure(Map* map, int x, int y, std::string building_type) {
    if (!map) return false;
    // Must build on 4-neighbor cell relative to character
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) return false;
    if (!is_adjacent_four(cur_x, cur_y, x, y)) return false;
    
    Cell* cell = map->get_cell(x, y);
    if (!cell || !cell->is_empty()) return false;
    
    Building* new_building = nullptr;
    int gold_cost = 0;
    int wood_cost = 0;
    int stone_cost = 0;
    
    if (building_type == "gold_building") {
        gold_cost = 50;
        wood_cost = 2;
        if (gold >= gold_cost && wood >= wood_cost) {
            new_building = new Gold_building("Gold Building", Gold_building_type::SMALL_FIELD, is_friendly_side());
            add_gold(-gold_cost);
            add_wood(-wood_cost);
            std::cout << "Built gold building for " << gold_cost << " gold and " << wood_cost << " wood" << std::endl;
        } else {
            std::cout << "Not enough resources for gold building! Need: " << gold_cost << " gold, " << wood_cost << " wood" << std::endl;
        }
    } else if (building_type == "exp_building") {
        gold_cost = 75;
        wood_cost = 1;
        stone_cost = 1;
        if (gold >= gold_cost && wood >= wood_cost && stone >= stone_cost) {
            new_building = new Exp_building("Exp Building", Exp_building_type::SMALL_CAMP, is_friendly_side());
            add_gold(-gold_cost);
            add_wood(-wood_cost);
            add_stone(-stone_cost);
            std::cout << "Built experience building for " << gold_cost << " gold, " << wood_cost << " wood, " << stone_cost << " stone" << std::endl;
        } else {
            std::cout << "Not enough resources for experience building! Need: " << gold_cost << " gold, " << wood_cost << " wood, " << stone_cost << " stone" << std::endl;
        }
    } else if (building_type == "attack_building") {
        gold_cost = 50;
        stone_cost = 2;
        if (gold >= gold_cost && stone >= stone_cost) {
            new_building = new Attack_building("Attack Tower", Attack_building_type::SMALL_TOWER, is_friendly_side());
            add_gold(-gold_cost);
            add_stone(-stone_cost);
            std::cout << "Built attack tower for " << gold_cost << " gold, " << stone_cost << " stone" << std::endl;
        } else {
            std::cout << "Not enough resources for attack tower! Need: " << gold_cost << " gold, " << stone_cost << " stone" << std::endl;
        }
    }
    
    if (new_building) {
        cell->set_entity(new_building);
        return true;
    }
    
    return false;
}

bool Player::attack_entity(Map* map, int x, int y) {
    if (!map || !character) return false;
    // Must attack a 4-neighbor cell relative to character
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) return false;
    if (!is_adjacent_four(cur_x, cur_y, x, y)) return false;
    
    Cell* cell = map->get_cell(x, y);
    if (!cell || cell->is_empty()) return false;
    
    Entity* target = cell->get_entity();
    Character* target_char = dynamic_cast<Character*>(target);
    Citadel* target_citadel = dynamic_cast<Citadel*>(target);
    
    if (target_char && target_char != character) {
        // Attack character
        int dmg = character->get_damage();
        target_char->take_damage(dmg);
        std::cout << "Attacking character at (" << x << ", " << y << ") for " << dmg << " damage" << std::endl;
        if (!target_char->is_alive()) {
            cell->set_entity(nullptr);
            delete target_char;
            std::cout << "Target eliminated." << std::endl;
        }
        return true;
    } else if (target_citadel && target_citadel->is_friendly() != is_friendly_side()) {
        // Attack enemy citadel
        int dmg = character->get_damage();
        target_citadel->take_damage(dmg);
        std::cout << "Attacking citadel at (" << x << ", " << y << ") for " << dmg << " damage" << std::endl;
        
        if (target_citadel->is_destroyed()) {
            cell->set_entity(nullptr);
            delete target_citadel;
            std::cout << "Enemy citadel destroyed! Victory!" << std::endl;
        }
        return true;
    }
    
    return false;
}

void Player::add_gold(int amount) {
    gold += amount;
    if (gold < 0) gold = 0;
}

void Player::add_experience(int amount) {
    character->add_experience(amount);
}


void Player::add_wood(int amount) {
    wood += amount;
    if (wood < 0) wood = 0;
}

void Player::add_stone(int amount) {
    stone += amount;
    if (stone < 0) stone = 0;
}


void Player::process_tower_attacks(Map* map) {
    if (!map) return;
    
    // Find all attack buildings
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Attack_building* tower = dynamic_cast<Attack_building*>(cell->get_entity());
                if (tower && tower->is_friendly() == is_friendly_side()) {
                    // This is our tower, check for enemies in 8 surrounding cells
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue;
                            
                            int target_x = x + dx;
                            int target_y = y + dy;
                            
                            if (target_x >= 0 && target_y >= 0 && target_x < map->get_width() && target_y < map->get_height()) {
                                Cell* target_cell = map->get_cell(target_x, target_y);
                                if (target_cell && !target_cell->is_empty()) {
                                    Character* enemy = dynamic_cast<Character*>(target_cell->get_entity());
                                    if (enemy && enemy->is_friendly() != is_friendly_side()) {
                                        // Found enemy character, attack it
                                        int damage = tower->get_damage();
                                        enemy->take_damage(damage);
                                        std::cout << "Tower at (" << x << ", " << y << ") attacks enemy for " << damage << " damage!" << std::endl;
                                        
                                        if (!enemy->is_alive()) {
                                            target_cell->set_entity(nullptr);
                                            delete enemy;
                                            std::cout << "Enemy eliminated by tower!" << std::endl;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

bool Player::upgrade_building(Map* map, int dx, int dy) {
    if (!map) return false;
    
    // Must target 4-neighbor cell relative to character
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) return false;
    if (!is_adjacent_four(cur_x, cur_y, cur_x + dx, cur_y + dy)) return false;
    
    int x = cur_x + dx;
    int y = cur_y + dy;
    
    Cell* cell = map->get_cell(x, y);
    if (!cell || cell->is_empty()) return false;
    
    Entity* entity = cell->get_entity();
    if (!entity) return false;
    
    // Check if it's our building
    Building* building = dynamic_cast<Building*>(entity);
    if (!building || building->is_friendly() != is_friendly_side()) {
        std::cout << "No friendly building to upgrade at this location." << std::endl;
        return false;
    }
    
    // Check building type and upgrade cost
    int upgrade_cost = 0;
    Building* new_building = nullptr;
    
    Gold_building* gold_building = dynamic_cast<Gold_building*>(entity);
    if (gold_building) {
        upgrade_cost = 30;
        if (gold >= upgrade_cost) {
            new_building = new Gold_building("Gold Building", Gold_building_type::LARGE_FIELD, is_friendly_side());
        } else {
            std::cout << "Not enough gold to upgrade gold building! Need: " << upgrade_cost << " gold" << std::endl;
            return false;
        }
    }
    
    Exp_building* exp_building = dynamic_cast<Exp_building*>(entity);
    if (exp_building) {
        upgrade_cost = 50;
        if (gold >= upgrade_cost) {
            new_building = new Exp_building("Exp Building", Exp_building_type::LARGE_CAMP, is_friendly_side());
        } else {
            std::cout << "Not enough gold to upgrade experience building! Need: " << upgrade_cost << " gold" << std::endl;
            return false;
        }
    }
    
    Attack_building* attack_building = dynamic_cast<Attack_building*>(entity);
    if (attack_building) {
        upgrade_cost = 40;
        if (gold >= upgrade_cost) {
            new_building = new Attack_building("Attack Tower", Attack_building_type::LARGE_TOWER, is_friendly_side());
        } else {
            std::cout << "Not enough gold to upgrade attack tower! Need: " << upgrade_cost << " gold" << std::endl;
            return false;
        }
    }
    
    if (new_building) {
        // Replace old building with upgraded one
        cell->set_entity(new_building);
        add_gold(-upgrade_cost);
        delete entity; // Delete old building
        std::cout << "Building upgraded for " << upgrade_cost << " gold!" << std::endl;
        return true;
    }
    
    std::cout << "This building cannot be upgraded or is already at maximum level." << std::endl;
    return false;
}

void Player::collect_resources_from_buildings(Map* map) {
    if (!map) return;
    
    // Go through entire map and find our buildings
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Entity* entity = cell->get_entity();
                Gold_building* gold_building = dynamic_cast<Gold_building*>(entity);
                Exp_building* exp_building = dynamic_cast<Exp_building*>(entity);
                
                if (gold_building && (gold_building->is_friendly() == is_friendly_side())) {
                    add_gold(gold_building->get_gold_per_turn());
                } else if (exp_building && (exp_building->is_friendly() == is_friendly_side())) {
                    add_experience(exp_building->get_exp_per_turn());
                }
            }
        }
    }
}

bool Player::has_won(Map* map) {
    if (!map) return false;
    
    // Look for enemy citadel
    for (int y = 0; y < map->get_height(); y++) {
        for (int x = 0; x < map->get_width(); x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Citadel* citadel = dynamic_cast<Citadel*>(cell->get_entity());
                if (citadel && !citadel->is_friendly()) {
                    return false; // Enemy citadel still alive
                }
            }
        }
    }
    
    return true; // Enemy citadel destroyed
}
