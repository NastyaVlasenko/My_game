#include "exp_building.h"

Exp_building::Exp_building(std::string name, Exp_building_type type, bool friendly)
    :Building(friendly, name), type(type) {
    if (type == Exp_building_type::SMALL_CAMP) {
        exp_per_turn = 100;
    }
    else if (type == Exp_building_type::LARGE_CAMP) {
        exp_per_turn = 150;
    }
}

char Exp_building::get_symbol() const {
    switch(type) {
        case Exp_building_type::SMALL_CAMP: return 'm';
        case Exp_building_type::LARGE_CAMP: return 'M';
        default: return '#';
    }
}

std::string Exp_building::get_color() const {
    if (is_friendly()) {return "\033[38;5;27m";}
    else {return "\033[38;5;160m";}
}
