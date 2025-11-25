SOURCES=$(shell find src -type f)
# OBJECTS=$(patsubst src/%.cpp,obj/%.o,$(SOURCES))
EXES=$(patsubst src/day%.cpp,bin/aoc15.%,$(SOURCES))
RUNTARGETS=$(patsubst bin/aoc15.%,run15.%,$(EXES))
TESTTARGETS=$(patsubst bin/aoc15.%,test15.%,$(EXES))

CXXFLAGS=-std=c++23 -Wall -Wextra -Wpedantic -fsanitize=address -g

default: $(EXES)

bin/aoc15.%: src/day%.cpp
	g++ $(CXXFLAGS) -o $@ $<

run: $(RUNTARGETS)

test: $(TESTTARGETS)

run15.%: bin/aoc15.% input/day%.txt
	$^

test15.%: bin/aoc15.% input/test%.txt
	$^

input/day%.txt:
	python get_data.py $@

clean:
	rm -rf bin

.PHONY: clean run test
