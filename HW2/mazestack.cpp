/*
 * mazestack.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <string>
#include <stack>
#include <cassert>

using namespace std;

class Coord
{
public:
  Coord(int rr, int cc) : m_r(rr), m_c(cc) {}
  int r() const { return m_r; }
  int c() const { return m_c; }
private:
  int m_r;
  int m_c;
};

bool pathExists(string maze[], int nRows, int nCols,
                int sr, int sc, int er, int ec);
// Return true if there is a path from (sr,sc) to (er,ec)
// through the maze; return false otherwise

bool pathExists(string maze[], int nRows, int nCols,
                int sr, int sc, int er, int ec)
{
  stack<Coord> coordStack;
  // initialize a 2d boolean array to mark discovered nodes
  bool **discovered = new bool *[nRows];
  for (int i = 0; i < nRows; i += 1) {
    discovered[i] = new bool[nCols];
    for (int j = 0; j < nCols; j += 1)
      discovered[i][j] = false;  // initialize all coordinates as undiscovered
  }
  coordStack.push(Coord(sr, sc));
  discovered[sr][sc] = true;
  while (!coordStack.empty()) {
    Coord curr = coordStack.top();  // check the next coord
    // cerr << "(" << curr.r() << "," << curr.c() << ")" << endl;
    coordStack.pop();
    if (curr.r() == er && curr.c() == ec)  return true;  // arrived

    // South
    Coord s = Coord(curr.r() + 1, curr.c());
    if (maze[s.r()][s.c()] != 'X' && !discovered[s.r()][s.c()]) {
      coordStack.push(s);
      discovered[s.r()][s.c()] = true;
    }
    // West
    Coord w = Coord(curr.r(), curr.c() - 1);
    if (maze[w.r()][w.c()] != 'X' && !discovered[w.r()][w.c()]) {
      coordStack.push(w);
      discovered[w.r()][w.c()] = true;
    }
    // North
    Coord n = Coord(curr.r() - 1, curr.c());
    if (maze[n.r()][n.c()] != 'X' && !discovered[n.r()][n.c()]) {
      coordStack.push(n);
      discovered[n.r()][n.c()] = true;
    }
    // East
    Coord e = Coord(curr.r(), curr.c() + 1);
    if (maze[e.r()][e.c()] != 'X' && !discovered[e.r()][e.c()]) {
      coordStack.push(e);
      discovered[e.r()][e.c()] = true;
    }
  }
  return false;
}

int main()
{
  string maze[10] = {
      "XXXXXXXXXX",
      "X....X...X",
      "X.XX.XX..X",
      "XXX....X.X",
      "X.XXX.XXXX",
      "X.X...X..X",
      "X...X.X..X",
      "XXXXX.X.XX",
      "X........X",
      "XXXXXXXXXX"
  };
  if (pathExists(maze, 10,10, 3,5, 8,8))
      cout << "Solvable!" << endl;
  else
      cout << "Out of luck!" << endl;

  string noWhereToGo[3] = {
      "XXXXX",
      "X.X.X",
      "XXXXX"};
  assert(pathExists(noWhereToGo, 3, 5, 1, 1, 1, 4) == false);

  string instantVictory[3] = {
      "XXX",
      "X.X",
      "XXX"};
  assert(pathExists(instantVictory, 3, 3, 1, 1, 1, 1) == true);

  string youngsDoubleSlit[10] = {
      "XXXXX",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "X.X.X",
      "XXXXX"};
  assert(pathExists(youngsDoubleSlit, 10, 5, 1, 1, 8, 3) == false);

  string complex[10] = {
      "XXXXXXXXXX",
      "X....X...X",
      "X.XX.XX..X",
      "XXX....X.X",
      "X.XXX.XXXX",
      "X.X...X..X",
      "X...X.X..X",
      "XXXXX.X.XX",
      "X........X",
      "XXXXXXXXXX"};
  assert(pathExists(complex, 10, 10, 3, 5, 8, 8) == true);

  cout << "passed all tests" << endl;
}
