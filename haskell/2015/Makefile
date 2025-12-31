SOURCES=$(shell find src -type f)
EXES=$(patsubst src/day%.hs,bin/aoc15.%,$(SOURCES))
RUNTARGETS=$(patsubst bin/aoc15.%,run15.%,$(EXES))
TESTTARGETS=$(patsubst bin/aoc15.%,test15.%,$(EXES))

HFLAGS=-odir obj -hidir obj -i../util -package ghc

default: $(EXES)

bin/aoc15.%: src/day%.hs ../util/Util.hs obj bin
	ghc $(HFLAGS) -o $@ $<

run: $(RUNTARGETS)

test: $(TESTTARGETS)

run15.%: bin/aoc15.% input/day%.txt
	$^

test15.%: bin/aoc15.% input/test%.txt
	$^

input/day%.txt:
	python get_data.py $@

clean:
	rm -rf bin obj

obj:
	mkdir obj

bin:
	mkdir bin

.PHONY: clean run test
