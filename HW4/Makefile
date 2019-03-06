#
# Makefile
# Jim Zenn, 2019-03-04 12:06
#

CC=clang++

all: test list sorts
	./test
	./list
	./sorts

test: test.cpp
	$(CC) test.cpp -o test

list: list-complete.cpp
	$(CC) list-complete.cpp -o list

sorts: sorts.cpp
	$(CC) sorts.cpp -o sorts

# vim:ft=make
#
