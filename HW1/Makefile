#
# Makefile
# Jim Zenn, 2019-01-18 08:23
#

CC=clang++

.PHONY: all
all: testSet testnewSet testSSNSet
	./testSet
	./testnewSet
	./testSSNSet

testSet: testSet.cpp Set.cpp
	$(CC) testSet.cpp Set.cpp -Wall -Wextra -o testSet

testnewSet: testSet.cpp newSet.cpp
	$(CC) testSet.cpp newSet.cpp -Wall -Wextra -o testnewSet

testSSNSet: testSSNSet.cpp Set.cpp SSNSet.cpp
	$(CC) testSSNSet.cpp Set.cpp SSNSet.cpp -Wall -Wextra -o testSSNSet

testnewSSNSet: testSSNSet.cpp newSet.cpp SSNSet.cpp
	$(CC) testSSNSet.cpp newSet.cpp SSNSet.cpp -Wall -Wextra -o testnewSSNSet

# vim:ft=make
#
