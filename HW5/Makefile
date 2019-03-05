#
# Makefile
# Jim Zenn, 2019-03-04 12:06
#

all: test1
	./test1

test1: test1.o
	$(CC) test1.o -o test1

test1.o: Set.h
	$(CC) -c test1.cpp -o test1.o


# vim:ft=make
#
