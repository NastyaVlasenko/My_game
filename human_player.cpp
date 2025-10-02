#include "human_player.h"
#include "render.h"
#include <iostream>
#include <limits>
#include <cctype>

HumanPlayer::HumanPlayer(Character* character) : Player(character) {
}

void HumanPlayer::make_turn(Map* map) {
    if (!map || !character) return;
    
    bool turn_complete = false;
    
    while (!turn_complete) {
        display_map(map);
        display_resources();
        display_menu();
        
        int choice = get_user_choice();
        
        switch (choice) {
            case 1:
                handle_movement(map);
                turn_complete = true;
                break;
            case 2:
                handle_harvest(map);
                turn_complete = true;
                break;
            case 3:
                handle_build(map);
                turn_complete = true;
                break;
            case 4:
                handle_attack(map);
                turn_complete = true;
                break;
            case 5:
                handle_upgrade(map);
                turn_complete = true;
                break;
            case 6:
                turn_complete = true; // Skip turn
                break;
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
                break;
        }
    }
}

void HumanPlayer::display_menu() {
    std::cout << "\n=== YOUR TURN ===" << std::endl;
    std::cout << "1. Move" << std::endl;
    std::cout << "2. Harvest resource" << std::endl;
    std::cout << "3. Build structure" << std::endl;
    std::cout << "4. Attack" << std::endl;
    std::cout << "5. Upgrade building" << std::endl;
    std::cout << "6. Skip turn" << std::endl;
    std::cout << "Choose action: ";
}

void HumanPlayer::display_map(Map* map) {
    std::cout << "\n";
    Render::print_map(map);
    std::cout << "\n";
}

void HumanPlayer::display_resources() {
    std::cout << "Gold: " << gold << " | Experience: " << get_experience() 
              << " | Wood: " << wood << " | Stone: " << stone << std::endl;
}

int HumanPlayer::get_user_choice() {
    int choice;
    std::cin >> choice;
    
    // Очищаем буфер ввода
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    return choice;
}

void HumanPlayer::handle_movement(Map* map) {
    std::cout << "Enter direction (W/A/S/D): ";
    char dir;
    std::cin >> dir;
    dir = std::tolower(dir);
    int cur_x = -1, cur_y = -1;
    if (!find_character_position(map, cur_x, cur_y)) { std::cout << "Character not found." << std::endl; return; }
    int nx = cur_x, ny = cur_y;
    if (dir == 'w') ny -= 1;
    else if (dir == 's') ny += 1;
    else if (dir == 'a') nx -= 1;
    else if (dir == 'd') nx += 1;
    else { std::cout << "Invalid direction." << std::endl; return; }
    if (move_character(map, nx, ny)) {
        std::cout << "Moved." << std::endl;
    } else {
        std::cout << "Cannot move." << std::endl;
    }
}

void HumanPlayer::handle_harvest(Map* map) {
    std::cout << "Choose direction to harvest (W/A/S/D): ";
    char dir; std::cin >> dir; dir = std::tolower(dir);
    int cx=-1, cy=-1; if (!find_character_position(map, cx, cy)) { std::cout << "Character not found." << std::endl; return; }
    int tx=cx, ty=cy;
    if (dir=='w') ty -= 1; else if (dir=='s') ty += 1; else if (dir=='a') tx -= 1; else if (dir=='d') tx += 1; else { std::cout << "Invalid direction." << std::endl; return; }
    if (harvest_resource(map, tx, ty)) {
        std::cout << "Resource harvested!" << std::endl;
    } else {
        std::cout << "Cannot harvest resource at the specified position." << std::endl;
    }
}

void HumanPlayer::handle_build(Map* map) {
    std::cout << "Choose building type:" << std::endl;
    std::cout << "1. Gold building (50 gold, 2 wood)" << std::endl;
    std::cout << "2. Experience building (75 gold, 1 wood, 1 stone)" << std::endl;
    std::cout << "3. Attack tower (50 gold, 2 stone)" << std::endl;
    std::cout << "Choice: ";
    
    int building_choice;
    std::cin >> building_choice;
    
    std::cout << "Choose direction to build (W/A/S/D): ";
    char dir; std::cin >> dir; dir = std::tolower(dir);
    int cx=-1, cy=-1; if (!find_character_position(map, cx, cy)) { std::cout << "Character not found." << std::endl; return; }
    int x=cx, y=cy;
    if (dir=='w') y -= 1; else if (dir=='s') y += 1; else if (dir=='a') x -= 1; else if (dir=='d') x += 1; else { std::cout << "Invalid direction." << std::endl; return; }
    
    std::string building_type;
    if (building_choice == 1) {
        building_type = "gold_building";
    } else if (building_choice == 2) {
        building_type = "exp_building";
    } else if (building_choice == 3) {
        building_type = "attack_building";
    } else {
        std::cout << "Invalid building choice." << std::endl;
        return;
    }
    
    if (build_structure(map, x, y, building_type)) {
        std::cout << "Building constructed!" << std::endl;
    } else {
        std::cout << "Cannot build at the specified position." << std::endl;
    }
}

void HumanPlayer::handle_attack(Map* map) {
    std::cout << "Choose direction to attack (W/A/S/D): ";
    char dir; std::cin >> dir; dir = std::tolower(dir);
    int cx=-1, cy=-1; if (!find_character_position(map, cx, cy)) { std::cout << "Character not found." << std::endl; return; }
    int x=cx, y=cy;
    if (dir=='w') y -= 1; else if (dir=='s') y += 1; else if (dir=='a') x -= 1; else if (dir=='d') x += 1; else { std::cout << "Invalid direction." << std::endl; return; }
    
    if (attack_entity(map, x, y)) {
        std::cout << "Attack executed!" << std::endl;
    } else {
        std::cout << "Cannot attack at the specified position." << std::endl;
    }
}

void HumanPlayer::handle_upgrade(Map* map) {
    std::cout << "Choose direction to upgrade building (W/A/S/D): ";
    char dir; std::cin >> dir; dir = std::tolower(dir);
    int cx=-1, cy=-1; if (!find_character_position(map, cx, cy)) { std::cout << "Character not found." << std::endl; return; }
    
    int dx = 0, dy = 0;
    if (dir=='w') dy = -1; else if (dir=='s') dy = 1; else if (dir=='a') dx = -1; else if (dir=='d') dx = 1; else { std::cout << "Invalid direction." << std::endl; return; }
    
    if (upgrade_building(map, dx, dy)) {
        std::cout << "Building upgraded successfully!" << std::endl;
    } else {
        std::cout << "Cannot upgrade building at the specified position." << std::endl;
    }
}
