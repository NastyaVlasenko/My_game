#pragma once
#include <string>

class Level {
private:
    int number;
    std::string name;
    std::string filename;
    bool completed;
    
public:
    Level(int num, const std::string& level_name, const std::string& file);
    
    int get_number() const { return number; }
    std::string get_name() const { return name; }
    std::string get_filename() const { return filename; }
    bool is_completed() const { return completed; }
    
    void set_completed(bool status) { completed = status; }
};
