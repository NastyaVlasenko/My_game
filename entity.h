#pragma once

#include <string>

class Entity{
private:
    std::string name;

public:
    Entity(std::string name);
    virtual ~Entity() = default;
    std::string get_name() const;
    void set_name(const std::string& name);

    virtual char get_symbol() const = 0;
    virtual std::string get_color() const = 0;
};
