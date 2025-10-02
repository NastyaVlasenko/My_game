#pragma once
#include <string>

enum class Biome_type {
    PLAINS,
    DESERT,
    TAIGA
};

class Biome {
private:
    Biome_type type;
    std::string name;

public:
    Biome(Biome_type type, const std::string& name);
    
    Biome_type get_type() const { return type; }
    std::string get_name() const { return name; }
    std::string get_empty_cell_color() const;
};
