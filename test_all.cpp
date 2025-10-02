#include <gtest/gtest.h>
#include "entity.h"
#include "character.h"
#include "building.h"
#include "cell.h"
#include "map.h"
#include "biome.h"
#include "level.h"
#include "levelmanager.h"

// ENTITY TESTS

// Mock class for testing Entity
class MockEntity : public Entity {
public:
    MockEntity(const std::string& name) : Entity(name) {}
    
    char get_symbol() const override {
        return 'M';
    }
    
    std::string get_color() const override {
        return "red";
    }
};

class EntityTest : public ::testing::Test {
protected:
    void SetUp() override {
        entity = new MockEntity("TestEntity");
    }
    
    void TearDown() override {
        delete entity;
    }
    
    Entity* entity;
};

TEST_F(EntityTest, Constructor) {
    EXPECT_EQ(entity->get_name(), "TestEntity");
}

TEST_F(EntityTest, SetName) {
    entity->set_name("NewName");
    EXPECT_EQ(entity->get_name(), "NewName");
}

TEST_F(EntityTest, GetSymbol) {
    EXPECT_EQ(entity->get_symbol(), 'M');
}

TEST_F(EntityTest, GetColor) {
    EXPECT_EQ(entity->get_color(), "red");
}

// CHARACTER TESTS

class CharacterTest : public ::testing::Test {
protected:
    void SetUp() override {
        friendly_char = new Character("FriendlyKnight", Character_type::KNIGHT, 100, 10, true);
        enemy_char = new Character("EnemyKnight", Character_type::KNIGHT, 80, 8, false);
    }
    
    void TearDown() override {
        delete friendly_char;
        delete enemy_char;
    }
    
    Character* friendly_char;
    Character* enemy_char;
};

TEST_F(CharacterTest, Constructor) {
    EXPECT_EQ(friendly_char->get_name(), "FriendlyKnight");
    EXPECT_EQ(friendly_char->get_hp(), 100);
    EXPECT_EQ(friendly_char->get_max_hp(), 100);
    EXPECT_EQ(friendly_char->get_damage(), 10);
    EXPECT_EQ(friendly_char->get_level(), 1);
    EXPECT_EQ(friendly_char->get_experience(), 0);
    EXPECT_TRUE(friendly_char->is_friendly());
    EXPECT_TRUE(friendly_char->is_alive());
}

TEST_F(CharacterTest, EnemyCharacter) {
    EXPECT_EQ(enemy_char->get_name(), "EnemyKnight");
    EXPECT_EQ(enemy_char->get_hp(), 80);
    EXPECT_EQ(enemy_char->get_max_hp(), 80);
    EXPECT_EQ(enemy_char->get_damage(), 8);
    EXPECT_FALSE(enemy_char->is_friendly());
    EXPECT_TRUE(enemy_char->is_alive());
}

TEST_F(CharacterTest, TakeDamage) {
    friendly_char->take_damage(30);
    EXPECT_EQ(friendly_char->get_hp(), 70);
    EXPECT_TRUE(friendly_char->is_alive());
}

TEST_F(CharacterTest, TakeDamageToDeath) {
    friendly_char->take_damage(100);
    EXPECT_EQ(friendly_char->get_hp(), 0);
    EXPECT_FALSE(friendly_char->is_alive());
}

TEST_F(CharacterTest, TakeExcessiveDamage) {
    friendly_char->take_damage(150);
    EXPECT_EQ(friendly_char->get_hp(), 0);
    EXPECT_FALSE(friendly_char->is_alive());
}

TEST_F(CharacterTest, GetSymbol) {
    EXPECT_EQ(friendly_char->get_symbol(), '@');
    EXPECT_EQ(enemy_char->get_symbol(), '@');
}

TEST_F(CharacterTest, GetColor) {
    EXPECT_EQ(friendly_char->get_color(), "\033[38;5;27m");
    EXPECT_EQ(enemy_char->get_color(), "\033[38;5;160m");
}

TEST_F(CharacterTest, AddExperience) {
    friendly_char->add_experience(500);
    EXPECT_EQ(friendly_char->get_experience(), 500);
    EXPECT_EQ(friendly_char->get_level(), 1);
}

TEST_F(CharacterTest, LevelUp) {
    friendly_char->add_experience(1000);
    EXPECT_EQ(friendly_char->get_level(), 2);
    EXPECT_EQ(friendly_char->get_experience(), 0);
    EXPECT_EQ(friendly_char->get_damage(), 11); // base_damage + level - 1
}

TEST_F(CharacterTest, MultipleLevelUps) {
    friendly_char->add_experience(2500);
    EXPECT_EQ(friendly_char->get_level(), 3);
    EXPECT_EQ(friendly_char->get_experience(), 500);
    EXPECT_EQ(friendly_char->get_damage(), 12); // base_damage + level - 1
}

TEST_F(CharacterTest, NegativeExperience) {
    friendly_char->add_experience(500);
    friendly_char->add_experience(-100);
    EXPECT_EQ(friendly_char->get_experience(), 400); // 500 - 100 = 400
}

TEST_F(CharacterTest, NegativeExperienceClampedToZero) {
    friendly_char->add_experience(100);
    friendly_char->add_experience(-200); 
    EXPECT_EQ(friendly_char->get_experience(), 0);
}

// BUILDING TESTS

class BuildingTest : public ::testing::Test {
protected:
    void SetUp() override {
        friendly_building = new Building(true, "FriendlyTower");
        enemy_building = new Building(false, "EnemyTower");
    }
    
    void TearDown() override {
        delete friendly_building;
        delete enemy_building;
    }
    
    Building* friendly_building;
    Building* enemy_building;
};

TEST_F(BuildingTest, Constructor) {
    EXPECT_EQ(friendly_building->get_name(), "FriendlyTower");
    EXPECT_TRUE(friendly_building->is_friendly());
    
    EXPECT_EQ(enemy_building->get_name(), "EnemyTower");
    EXPECT_FALSE(enemy_building->is_friendly());
}

TEST_F(BuildingTest, IsFriendly) {
    EXPECT_TRUE(friendly_building->is_friendly());
    EXPECT_FALSE(enemy_building->is_friendly());
}

TEST_F(BuildingTest, GetSymbol) {
    EXPECT_EQ(friendly_building->get_symbol(), 'B');
    EXPECT_EQ(enemy_building->get_symbol(), 'B');
}

TEST_F(BuildingTest, GetColor) {
    EXPECT_EQ(friendly_building->get_color(), "\033[38;5;34m");
    EXPECT_EQ(enemy_building->get_color(), "\033[38;5;196m");
}

// CELL TESTS

class CellTest : public ::testing::Test {
protected:
    void SetUp() override {
        cell = new Cell(5, 10);
        character = new Character("TestKnight", Character_type::KNIGHT, 100, 10, true);
    }
    
    void TearDown() override {
        delete cell;
        delete character;
    }
    
    Cell* cell;
    Character* character;
};

TEST_F(CellTest, Constructor) {
    EXPECT_EQ(cell->get_x(), 5);
    EXPECT_EQ(cell->get_y(), 10);
    EXPECT_TRUE(cell->is_empty());
    EXPECT_EQ(cell->get_entity(), nullptr);
}

TEST_F(CellTest, SetEntity) {
    cell->set_entity(character);
    EXPECT_FALSE(cell->is_empty());
    EXPECT_EQ(cell->get_entity(), character);
}

TEST_F(CellTest, SetEntityToNull) {
    cell->set_entity(character);
    cell->set_entity(nullptr);
    EXPECT_TRUE(cell->is_empty());
    EXPECT_EQ(cell->get_entity(), nullptr);
}

TEST_F(CellTest, GetCoordinates) {
    EXPECT_EQ(cell->get_x(), 5);
    EXPECT_EQ(cell->get_y(), 10);
}

// MAP TESTS

class MapTest : public ::testing::Test {
protected:
    void SetUp() override {
        biome = new Biome(Biome_type::PLAINS, "TestPlains");
        map = new Map(10, 8, biome);
        character = new Character("TestKnight", Character_type::KNIGHT, 100, 10, true);
    }
    
    void TearDown() override {
        delete map; // This will also delete the biome
        delete character;
    }
    
    Map* map;
    Biome* biome;
    Character* character;
};

TEST_F(MapTest, Constructor) {
    EXPECT_EQ(map->get_width(), 10);
    EXPECT_EQ(map->get_height(), 8);
    EXPECT_EQ(map->get_biome(), biome);
}

TEST_F(MapTest, GetCellValid) {
    Cell* cell = map->get_cell(5, 3);
    ASSERT_NE(cell, nullptr);
    EXPECT_EQ(cell->get_x(), 5);
    EXPECT_EQ(cell->get_y(), 3);
}

TEST_F(MapTest, GetCellOutOfBounds) {
    EXPECT_EQ(map->get_cell(-1, 3), nullptr);
    EXPECT_EQ(map->get_cell(5, -1), nullptr);
    EXPECT_EQ(map->get_cell(10, 3), nullptr);
    EXPECT_EQ(map->get_cell(5, 8), nullptr);
    EXPECT_EQ(map->get_cell(15, 15), nullptr);
}

TEST_F(MapTest, GetCellBoundary) {
    EXPECT_NE(map->get_cell(0, 0), nullptr);
    EXPECT_NE(map->get_cell(9, 7), nullptr);
    EXPECT_EQ(map->get_cell(10, 7), nullptr);
    EXPECT_EQ(map->get_cell(9, 8), nullptr);
}

TEST_F(MapTest, SetBiome) {
    Biome* new_biome = new Biome(Biome_type::DESERT, "TestDesert");
    map->set_biome(new_biome);
    EXPECT_EQ(map->get_biome(), new_biome);
    delete new_biome;
}

TEST_F(MapTest, CellOperations) {
    Cell* cell = map->get_cell(3, 4);
    ASSERT_NE(cell, nullptr);
    
    EXPECT_TRUE(cell->is_empty());
    cell->set_entity(character);
    EXPECT_FALSE(cell->is_empty());
    EXPECT_EQ(cell->get_entity(), character);
}

// BIOME TESTS

class BiomeTest : public ::testing::Test {
protected:
    void SetUp() override {
        plains_biome = new Biome(Biome_type::PLAINS, "TestPlains");
        desert_biome = new Biome(Biome_type::DESERT, "TestDesert");
        taiga_biome = new Biome(Biome_type::TAIGA, "TestTaiga");
    }
    
    void TearDown() override {
        delete plains_biome;
        delete desert_biome;
        delete taiga_biome;
    }
    
    Biome* plains_biome;
    Biome* desert_biome;
    Biome* taiga_biome;
};

TEST_F(BiomeTest, Constructor) {
    EXPECT_EQ(plains_biome->get_type(), Biome_type::PLAINS);
    EXPECT_EQ(plains_biome->get_name(), "TestPlains");
    
    EXPECT_EQ(desert_biome->get_type(), Biome_type::DESERT);
    EXPECT_EQ(desert_biome->get_name(), "TestDesert");
    
    EXPECT_EQ(taiga_biome->get_type(), Biome_type::TAIGA);
    EXPECT_EQ(taiga_biome->get_name(), "TestTaiga");
}

TEST_F(BiomeTest, GetType) {
    EXPECT_EQ(plains_biome->get_type(), Biome_type::PLAINS);
    EXPECT_EQ(desert_biome->get_type(), Biome_type::DESERT);
    EXPECT_EQ(taiga_biome->get_type(), Biome_type::TAIGA);
}

TEST_F(BiomeTest, GetName) {
    EXPECT_EQ(plains_biome->get_name(), "TestPlains");
    EXPECT_EQ(desert_biome->get_name(), "TestDesert");
    EXPECT_EQ(taiga_biome->get_name(), "TestTaiga");
}

TEST_F(BiomeTest, GetEmptyCellColor) {
    // Test that the method returns a non-empty string
    EXPECT_FALSE(plains_biome->get_empty_cell_color().empty());
    EXPECT_FALSE(desert_biome->get_empty_cell_color().empty());
    EXPECT_FALSE(taiga_biome->get_empty_cell_color().empty());
    
    // Test that different biomes have different colors
    EXPECT_NE(plains_biome->get_empty_cell_color(), desert_biome->get_empty_cell_color());
    EXPECT_NE(plains_biome->get_empty_cell_color(), taiga_biome->get_empty_cell_color());
    EXPECT_NE(desert_biome->get_empty_cell_color(), taiga_biome->get_empty_cell_color());
}

// LEVEL TESTS

class LevelTest : public ::testing::Test {
protected:
    void SetUp() override {
        level = new Level(1, "TestLevel", "test_level.txt");
    }
    
    void TearDown() override {
        delete level;
    }
    
    Level* level;
};

TEST_F(LevelTest, Constructor) {
    EXPECT_EQ(level->get_number(), 1);
    EXPECT_EQ(level->get_name(), "TestLevel");
    EXPECT_EQ(level->get_filename(), "test_level.txt");
    EXPECT_FALSE(level->is_completed());
}

TEST_F(LevelTest, GetNumber) {
    EXPECT_EQ(level->get_number(), 1);
}

TEST_F(LevelTest, GetName) {
    EXPECT_EQ(level->get_name(), "TestLevel");
}

TEST_F(LevelTest, GetFilename) {
    EXPECT_EQ(level->get_filename(), "test_level.txt");
}

TEST_F(LevelTest, IsCompleted) {
    EXPECT_FALSE(level->is_completed());
}

TEST_F(LevelTest, SetCompleted) {
    level->set_completed(true);
    EXPECT_TRUE(level->is_completed());
    
    level->set_completed(false);
    EXPECT_FALSE(level->is_completed());
}

TEST_F(LevelTest, MultipleSetCompleted) {
    level->set_completed(true);
    EXPECT_TRUE(level->is_completed());
    
    level->set_completed(true);
    EXPECT_TRUE(level->is_completed());
    
    level->set_completed(false);
    EXPECT_FALSE(level->is_completed());
}

// LEVEL MANAGER TESTS

class LevelManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        level_manager = new LevelManager();
    }
    
    void TearDown() override {
        delete level_manager;
    }
    
    LevelManager* level_manager;
};

TEST_F(LevelManagerTest, Constructor) {
    EXPECT_FALSE(level_manager->is_all_completed());
    EXPECT_TRUE(level_manager->has_next_level());
}

TEST_F(LevelManagerTest, GetCurrentLevel) {
    Level& current = level_manager->get_current_level();
    EXPECT_EQ(current.get_number(), 1);
    EXPECT_EQ(current.get_name(), "Plains");
    EXPECT_EQ(current.get_filename(), "Level1");
}

TEST_F(LevelManagerTest, HasNextLevel) {
    EXPECT_TRUE(level_manager->has_next_level());
    
    // Complete first level
    level_manager->complete_current_level();
    EXPECT_TRUE(level_manager->has_next_level());
    
    // Complete second level
    level_manager->complete_current_level();
    EXPECT_TRUE(level_manager->has_next_level());
    
    // Complete third level
    level_manager->complete_current_level();
    EXPECT_FALSE(level_manager->has_next_level());
}

TEST_F(LevelManagerTest, CompleteCurrentLevel) {
    Level& level1 = level_manager->get_current_level();
    EXPECT_FALSE(level1.is_completed());
    
    level_manager->complete_current_level();
    EXPECT_TRUE(level1.is_completed());
    
    // Should move to next level
    Level& level2 = level_manager->get_current_level();
    EXPECT_EQ(level2.get_number(), 2);
    EXPECT_EQ(level2.get_name(), "Desert");
}

TEST_F(LevelManagerTest, IsAllCompleted) {
    EXPECT_FALSE(level_manager->is_all_completed());
    
    // Complete all levels
    level_manager->complete_current_level(); // Level 1 -> 2
    level_manager->complete_current_level(); // Level 2 -> 3
    level_manager->complete_current_level(); // Level 3 -> all completed
    
    EXPECT_TRUE(level_manager->is_all_completed());
}

TEST_F(LevelManagerTest, LevelProgression) {
    // Level 1
    Level& level1 = level_manager->get_current_level();
    EXPECT_EQ(level1.get_number(), 1);
    EXPECT_EQ(level1.get_name(), "Plains");
    
    level_manager->complete_current_level();
    
    // Level 2
    Level& level2 = level_manager->get_current_level();
    EXPECT_EQ(level2.get_number(), 2);
    EXPECT_EQ(level2.get_name(), "Desert");
    
    level_manager->complete_current_level();
    
    // Level 3
    Level& level3 = level_manager->get_current_level();
    EXPECT_EQ(level3.get_number(), 3);
    EXPECT_EQ(level3.get_name(), "Taiga");
    
    level_manager->complete_current_level();
    
    // All completed
    EXPECT_TRUE(level_manager->is_all_completed());
}
