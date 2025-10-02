#include "barrier.h"

Barrier::Barrier (std::string name, Barrier_type type)
    : Entity(name), type(type) {}

char Barrier::get_symbol() const {
    switch(type) {
        case Barrier_type::WATER: return '~';
        case Barrier_type::THORNS: return '#';
        default: return '#';
    }
}

std::string Barrier::get_color() const {
    switch(type) {
        case Barrier_type::WATER: return "\033[38;5;39m";
        case Barrier_type::THORNS: return "\033[38;5;136m";
        default: return "";
    }
}
