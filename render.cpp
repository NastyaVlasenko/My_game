#include "render.h"
#include "attack_building.h"
#include "biome.h"

void Render::print_map(Map* map) {
    if (map == nullptr) {
        std::cerr << "Error: map is nullptr!" << std::endl;
        return;
    }

    int height = map->get_height();
    int width = map->get_width();

    if (height <= 0 || width <= 0) {
        std::cerr << "Error: invalid map dimensions!" << std::endl;
    }
    
    // First pass: find all attack buildings and mark their attack zones
    bool** attack_zones = new bool*[height];
    for (int y = 0; y < height; y++) {
        attack_zones[y] = new bool[width];
        for (int x = 0; x < width; x++) {
            attack_zones[y][x] = false;
        }
    }
    
    // Mark attack zones around towers
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            Cell* cell = map->get_cell(x, y);
            if (cell && !cell->is_empty()) {
                Attack_building* tower = dynamic_cast<Attack_building*>(cell->get_entity());
                if (tower) {
                    // Mark 8 surrounding cells as attack zones
                    for (int dy = -1; dy <= 1; dy++) {
                        for (int dx = -1; dx <= 1; dx++) {
                            if (dx == 0 && dy == 0) continue;
                            
                            int target_x = x + dx;
                            int target_y = y + dy;
                            
                            if (target_x >= 0 && target_y >= 0 && target_x < width && target_y < height) {
                                Cell* target_cell = map->get_cell(target_x, target_y);
                                if (target_cell && target_cell->is_empty()) {
                                    attack_zones[target_y][target_x] = true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }
    
    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            Cell* cell = map->get_cell(x, y);
            if (cell == nullptr) {
                continue;
            }
            if (cell->is_empty()) {
                if (attack_zones[y][x]) {
                    std::cout << "\033[38;5;160m*\033[0m "; // Red star for attack zone
                } else {
                    // Use biome color for empty cells
                    Biome* biome = map->get_biome();
                    if (biome) {
                        std::cout << biome->get_empty_cell_color() << ".\033[0m ";
                    } else {
                        std::cout << "\033[38;5;40m.\033[0m "; // Default green dot
                    }
                }
            }
            else {
                Entity* entity = cell->get_entity();
                if (entity != nullptr){
                    std::cout << entity->get_color() << entity->get_symbol() << "\033[0m ";
                }
            }
        }
        std::cout << std::endl;
    }
    
    // Clean up
    for (int y = 0; y < height; y++) {
        delete[] attack_zones[y];
    }
    delete[] attack_zones;
};
