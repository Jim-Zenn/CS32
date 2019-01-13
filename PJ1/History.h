/*
 * History.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef HISTORY_H
#define HISTORY_H

#include "globals.h"

class History
{
    public:
        History(int nRows, int nCols);
        bool record(int r, int c);
        void display() const;
    private:
        int m_rows;
        int m_cols;
        char grid[MAXROWS][MAXCOLS];
};

#endif /* !HISTORY_H */

