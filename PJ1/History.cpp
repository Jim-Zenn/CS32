/*
 * History.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#include <iostream>
#include <cstdlib>

#include "History.h"
#include "utilities.cpp"

using namespace std;

History::History(int nRows, int nCols) : m_rows(nRows), m_cols(nCols)
{
    // Fill the grid with dots
    for (int r = 0; r < m_rows; r++)
        for (int c = 0; c < m_cols; c++)
            grid[r][c] = '.';
}

bool History::record(int r, int c) 
{
    if (r < 1 || r > m_rows || c < 1 || c > m_cols) {
        // Out of bound
        return false;
    }

    char &cell = grid[r - 1][c - 1];

    if (cell == '.') {
        cell = 'A';
    } else if (cell >= 'A' && cell < 'Z') {
        cell += 1;
    } else if (cell == 'Z') {
        // pass
    } else {
        // Something is wrong.
        cout << "History grid contains an invalid character " << cell << endl;
        exit(1);
    }

    return true;
}

void History::display() const 
{
    // Draw the grid
    clearScreen();
    for (int r = 0; r < m_rows; r++)
    {
        for (int c = 0; c < m_cols; c++)
            cout << grid[r][c];
        cout << endl;
    }
    cout << endl;
}

