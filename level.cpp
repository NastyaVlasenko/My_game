#include "level.h"

Level::Level(int num, const std::string& level_name, const std::string& file) 
    : number(num), name(level_name), filename(file), completed(false) {
}
