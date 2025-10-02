PROJECT = play

LIBPROJECT = lib$(PROJECT).a

TESTPROJECT = test_$(PROJECT)

CXX = g++

AR = ar

AFLAGS = rsv

CXXFLAGS = -I. -std=c++17 -Wall -g -fPIC -Werror -Wpedantic

LDXXFLAGS = $(CXXFLAGS) -L. -l:$(LIBPROJECT)

TESTCXXFLAGS = -I. -std=c++17 -Wall -g -fPIC
LDGTESTFLAGS = $(TESTCXXFLAGS) -L. -l:$(LIBPROJECT) -lgtest -lgtest_main -pthread

DEPS=$(wildcard *.h)

OBJ=maploader.o map.o cell.o entity.o resource.o barrier.o citadel.o chest.o character.o building.o gold_building.o exp_building.o attack_building.o render.o player.o human_player.o ai_player.o game.o biome.o level.o levelmanager.o

TEST-OBJ=test_all.o

.PHONY: default

default: all

%.o: %.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(CXXFLAGS)

# Special rule for test files
test_all.o: test_all.cpp $(DEPS)
	$(CXX) -c -o $@ $< $(TESTCXXFLAGS)

$(LIBPROJECT): $(OBJ)
	$(AR) $(AFLAGS) $@ $^

$(PROJECT): main.o $(LIBPROJECT)
	$(CXX) -o $@ main.o $(LDXXFLAGS)

$(TESTPROJECT): $(LIBPROJECT) $(TEST-OBJ)
	$(CXX) -o $@ $(TEST-OBJ) $(LDGTESTFLAGS)

all: $(PROJECT)

# Test targets
test: $(TESTPROJECT)
	./$(TESTPROJECT)

.PHONY: clean cleanall test

clean:
	rm -f *.o

cleanall: clean
	rm -f $(PROJECT)
	rm -f $(LIBPROJECT)
	rm -f $(TESTPROJECT)

