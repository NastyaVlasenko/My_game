#pragma once

#include "map.h"
#include "entity.h"
#include <string>
#include "resource.h"
#include "barrier.h"
#include "citadel.h"
#include "chest.h"
#include "character.h"

class Maploader{
public:
    static Map* terrain_from_file(const std::string& file_path);
    static Map* entities_from_file(Map* map, const std::string& file_path);
};
