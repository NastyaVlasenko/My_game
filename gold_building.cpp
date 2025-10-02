#include "gold_building.h"

Gold_building::Gold_building(std::string name, Gold_building_type type, bool friendly)
    :Building(friendly, name), type(type) {
    if (type == Gold_building_type::SMALL_FIELD) {
        gold_per_turn = 5;
    }
    else if (type == Gold_building_type::LARGE_FIELD) {
        gold_per_turn = 10;
    }
}

char Gold_building::get_symbol() const {
    switch(type) {
        case Gold_building_type::SMALL_FIELD: return 'n';
        case Gold_building_type::LARGE_FIELD: return 'N';
        default: return '#';
    }
}

std::string Gold_building::get_color() const {
    if (is_friendly()) {return "\033[38;5;27m";}
    else {return "\033[38;5;160m";}
}
