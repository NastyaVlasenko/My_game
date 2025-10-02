# Tower Defense Game

A C++ tower defense game with multiple levels, AI opponents, and comprehensive unit testing.

## Features

- **Multiple Levels**: 3 different biomes (Plains, Desert, Taiga)
- **AI Opponent**: Smart AI player with strategic building and combat
- **Character System**: Level up system with experience and damage scaling
- **Building System**: Attack towers, resource generators, and defensive structures
- **Resource Management**: Gold, wood, and stone collection
- **Unit Testing**: Comprehensive Google Test suite

## Game Classes

### Core Classes
- `Entity` - Base class for all game objects
- `Character` - Player units with combat and leveling
- `Building` - Structures for defense and resource generation
- `Map` - Game world with cells and biomes
- `Cell` - Individual map positions

### Game Logic
- `Game` - Main game controller
- `Player` - Base player class
- `HumanPlayer` - Human player implementation
- `AIPlayer` - AI opponent with strategic behavior
- `LevelManager` - Handles level progression

### Specialized Buildings
- `AttackBuilding` - Defensive towers
- `GoldBuilding` - Gold generation
- `ExpBuilding` - Experience generation
- `Citadel` - Main base structure
- `Chest` - Treasure containers

## Building and Running

### Prerequisites
- C++17 compatible compiler (g++)
- Make
- Google Test (for unit tests)

### Compilation
```bash
# Build the game
make

# Run the game
./play

# Build and run tests
make test

# Clean build files
make cleanall
```

### Installing Google Test (Ubuntu)
```bash
sudo apt update
sudo apt install libgtest-dev cmake build-essential
```

## Project Structure

```
my_game/
├── *.cpp, *.h          # Source and header files
├── Level1, Level2, Level3  # Level definition files
├── test_all.cpp        # Unit tests
├── Makefile           # Build configuration
├── .gitignore         # Git ignore rules
└── README.md          # This file
```

## Testing

The project includes comprehensive unit tests covering:
- Entity creation and manipulation
- Character combat and leveling system
- Building functionality
- Map and cell operations
- Biome system
- Level management
- Game state management

Run tests with:
```bash
make test
```

## Game Mechanics

### Character System
- Characters start at level 1 with base damage
- Gain experience through combat and buildings
- Level up every 1000 experience points
- Damage increases with level

### Resource System
- **Gold**: Primary currency for building
- **Wood**: Construction material
- **Stone**: Advanced construction material

### Victory Conditions
- Destroy the enemy citadel to win
- Protect your own citadel from destruction

## License

This project is open source and available under the MIT License.
