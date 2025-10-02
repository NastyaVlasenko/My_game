#include "maploader.h"
#include "map.h"
#include "cell.h"
#include "character.h"
#include "citadel.h"
#include "chest.h"
#include "attack_building.h"
#include "barrier.h"
#include "biome.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

Map* Maploader::terrain_from_file(const std::string& file_path) {

    std::ifstream file(file_path);
    if (! file.is_open()) {
    std::cerr << "ERROR: Cannot open file: " << file_path << std::endl;
    return nullptr;
    }

    std::vector<std::vector<char>> temp_map;
    std::string line;
    Biome_type biome_type = Biome_type::PLAINS; // Default biome

    while (std::getline(file, line)) {
        if (line.empty() || line == "/CREATE MAP") continue;
        else if (line == "/PLACE ENTITIES") break;
        else if (line.substr(0, 7) == "/BIOME ") {
            std::string biome_name = line.substr(7);
            if (biome_name == "PLAINS") biome_type = Biome_type::PLAINS;
            else if (biome_name == "DESERT") biome_type = Biome_type::DESERT;
            else if (biome_name == "TAIGA") biome_type = Biome_type::TAIGA;
            continue;
        }
        // Don't skip lines that start with # - they are part of the map
        std::vector<char> row;
        for (char c : line) {
        row.push_back(c);
        }
        if (!row.empty()) {
            temp_map.push_back(row);
        }
    }

    if (temp_map.empty()) {
        std::cerr << "ERROR: Map data is empty" << std::endl;
        return nullptr;
    }

    int width = static_cast<int>(temp_map[0].size());
    int height = static_cast<int>(temp_map.size());
    

    // Create biome based on file
    std::string biome_name;
    switch(biome_type) {
        case Biome_type::PLAINS: biome_name = "Plains"; break;
        case Biome_type::DESERT: biome_name = "Desert"; break;
        case Biome_type::TAIGA: biome_name = "Taiga"; break;
    }
    Biome* biome = new Biome(biome_type, biome_name);
    Map* map = new Map(width, height, biome);

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            char cell_char = temp_map[y][x];
            Cell* cell = map->get_cell(x, y);
            Entity* entity = nullptr;
            switch (cell_char){
                case '.': break;
                case 'T':
                    entity = new Resource("Tree", Resource_type::TREE);
                    break;
                case 'O':
                    entity = new Resource("Rock", Resource_type::ROCK);
                    break;
                case '~':
                    entity = new Barrier("Water", Barrier_type::WATER);
                    break;
                case '#':
                    entity = new Barrier("Thorns", Barrier_type::THORNS);
                    break;
            }
            if (cell != nullptr) {
                cell->set_entity(entity);
            }
        }
    }
    return map;
}
Map* Maploader::entities_from_file(Map* map, const std::string& file_path){

    std::ifstream file(file_path);
    if (! file.is_open()) {
    std::cerr << "ERROR: Cannot open file: " << file_path << std::endl;
    return nullptr;
    }

    std::string line;
    bool read_entities = false;
    std::string name, type;
    bool friendly;
    int x, y;


    while (std::getline(file, line)) {
        if (line == "/PLACE ENTITIES") {read_entities = true; continue;}
        else if  (line.empty() || line == "/CREATE MAP") continue;
        else if (read_entities) {
            std::istringstream iss(line);
            if (!(iss >> name >> type >> friendly >> x >> y)) {
                std::cerr << "ERROR: Wrong data format" <<std::endl;
                return nullptr;
            }
            else {
                Cell* cell = map->get_cell(x, y);
                if (cell == nullptr) { 
                    continue; 
                }
                Entity* entity = nullptr;
                if (type == "CITADEL")
                    {entity = new Citadel(name, Citadel_type::CITADEL, 20, friendly);}                
                else if (type == "KNIGHT")
                    {entity = new Character(name, Character_type::KNIGHT, 5, 1, friendly);}                
                else if (type == "COMMON_CHEST")
                    {entity = new Chest(name, Chest_type::COMMON_CHEST);}
                else if (type == "RARE_CHEST")
                    {entity = new Chest(name, Chest_type::RARE_CHEST);}
                else if (type == "EPIC_CHEST")
                    {entity = new Chest(name, Chest_type::EPIC_CHEST);}                
                
                if (entity) {
                    cell->set_entity(entity);
                }
            }
        }
    }
    return map;
}
