#
# Makefile
# Jim Zenn, 2019-03-14 14:27
#

CC=clang++
CFLAGS=-std=c++17

.PHONY: all
all: cli test

.PHONY: clean
clean:
	rm -rf *.o cli test

cli: cli.o Genome.o GenomeMatcher.o
	$(CC) $(CFLAGS) cli.o Genome.o GenomeMatcher.o -o cli

test: test.o Genome.o GenomeMatcher.o
	$(CC) $(CFLAGS) test.o Genome.o GenomeMatcher.o -o test

cli.o: cli.cpp
	$(CC) $(CFLAGS) -c cli.cpp
	
test.o: test.cpp
	$(CC) $(CFLAGS) -c test.cpp

Genome.o: Genome.cpp
	$(CC) $(CFLAGS) -c Genome.cpp

GenomeMatcher.o: GenomeMatcher.cpp
	$(CC) $(CFLAGS) -c GenomeMatcher.cpp

# vim:ft=make
#
