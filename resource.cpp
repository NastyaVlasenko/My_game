#include "resource.h"

Resource::Resource(std::string name, Resource_type type)
    :Entity(name), type(type) {}

char Resource::get_symbol() const {
    switch(type) {
        case Resource_type::TREE: return 'T';
        case Resource_type::ROCK: return 'O';
        default: return '#';
    }
}

std::string Resource::get_color() const {
    switch(type) {
        case Resource_type::TREE: return "\033[38;5;22m" ;
        case Resource_type::ROCK: return "\033[38;5;245m";
        default: return "";
    }
}
