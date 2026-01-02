SOURCES=$(shell find src -type f)
EXES=$(patsubst src/day%.hs,bin/aoc15.%,$(SOURCES))
RUNTARGETS=$(patsubst bin/aoc15.%,run15.%,$(EXES))
TESTTARGETS=$(patsubst bin/aoc15.%,test15.%,$(EXES))

LATEST=src/$(shell ls -Art src | tail -n 1)
LATESTEXE=$(patsubst src/day%.hs,bin/aoc15.%,$(LATEST))
LATESTRUN=$(patsubst bin/aoc15.%,run15.%,$(LATESTEXE))
LATESTTEST=$(patsubst bin/aoc15.%,test15.%,$(LATESTEXE))

HFLAGS=-odir obj -hidir obj -i../util -package ghc

default: $(EXES)

bin/aoc15.%: src/day%.hs ../util/Util.hs | obj bin
	ghc $(HFLAGS) -hisuf $(patsubst src/day%.hs,%,$<).hi -osuf $(patsubst src/day%.hs,%,$<).o -o $@ $<

run: $(RUNTARGETS)

test: $(TESTTARGETS)

run15.%: bin/aoc15.% input/day%.txt
	$^

test15.%: bin/aoc15.% input/test%.txt
	$^

input/day%.txt:
	python get_data.py $@

latest: $(LATESTRUN)

testlatest: $(LATESTTEST)

clean:
	rm -rf bin obj

obj:
	mkdir obj

bin:
	mkdir bin

check:
	@echo $(LATEST)
	@echo $(LATESTEXE)
	@echo $(LATESTRUN)
	@echo $(LATESTTEST)

.PHONY: clean run test latest testlatest
.PRECIOUS: input/day%.txt
