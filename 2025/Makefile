SOURCES=$(shell find src -type f)
# OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(SOURCES))
EXES=$(patsubst src/day%.cpp,bin/aoc25.%,$(SOURCES))
RUNTARGETS=$(patsubst bin/aoc25.%,run25.%,$(EXES))
TESTTARGETS=$(patsubst bin/aoc25.%,test25.%,$(EXES))

CXXFLAGS=-std=c++23 -Wall -Wextra -Wpedantic -fsanitize=address -fsanitize=undefined -O3 -Iinclude

default: $(EXES)

bin/aoc25.%: src/day%.cpp Makefile
	g++ $(CXXFLAGS) -o $@ $<

run: $(RUNTARGETS)

test: $(TESTTARGETS)

run25.%: bin/aoc25.% input/day%.txt
	$^

test25.%: bin/aoc25.% input/test%.txt
	$^

input/day%.txt:
	python get_data.py $@

clean:
	rm -f bin/*

.PHONY: clean run test
