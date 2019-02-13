/*
 * maze.cpp
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
  maze[sr][sc] = '!';
  if (sr == er && sc == ec)
    return true;
  bool sy = false, wy = false, ny = false, ey = false;
  // South
  Coord s = Coord(sr + 1, sc);
  if (maze[s.r()][s.c()] != 'X' && maze[s.r()][s.c()] != '!' )
    sy = pathExists(maze, nRows, nCols, s.r(), s.c(), er, ec);
  // West
  Coord w = Coord(sr, sc - 1);
  if (maze[w.r()][w.c()] != 'X' && maze[s.r()][s.c()] != '!' )
    wy = pathExists(maze, nRows, nCols, w.r(), w.c(), er, ec);
  // North
  Coord n = Coord(sr - 1, sc);
  if (maze[n.r()][n.c()] != 'X' && maze[s.r()][s.c()] != '!' )
    ny = pathExists(maze, nRows, nCols, n.r(), n.c(), er, ec);
  // East
  Coord e = Coord(sr, sc + 1);
  if (maze[e.r()][e.c()] != 'X' && maze[e.r()][e.c()] != '!' )
    ey = pathExists(maze, nRows, nCols, e.r(), e.c(), er, ec);
  return sy || wy || ny || ey;
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
      "XXXXXXXXXX"};
  if (pathExists(maze, 10, 10, 3, 5, 8, 8))
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
