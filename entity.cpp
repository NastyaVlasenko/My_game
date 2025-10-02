#include "entity.h"

Entity::Entity(std::string name):
    name(name) {}

std::string Entity::get_name() const {return name;}

void Entity::set_name(const std::string& name) {
    this->name = name;
}
