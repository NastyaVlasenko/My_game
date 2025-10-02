#include "biome.h"

Biome::Biome(Biome_type type, const std::string& name)
    : type(type), name(name) {
}

std::string Biome::get_empty_cell_color() const {
    switch(type) {
        case Biome_type::PLAINS: return "\033[38;5;40m";  // Green
        case Biome_type::DESERT: return "\033[38;5;229m"; // Yellow
        case Biome_type::TAIGA: return "\033[38;5;15m";   // White
        default: return "\033[38;5;40m"; // Default to green
    }
}

