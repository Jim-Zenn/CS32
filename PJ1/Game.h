/*
 * Game.h
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */

#ifndef GAME_H
#define GAME_H

int decodeDirection(char dir);

class Arena;

class Game
{
  public:
        // Constructor/destructor
    Game(int rows, int cols, int nZombies);
    ~Game();

        // Mutators
    void play();

  private:
    Arena* m_arena;
};

#endif /* !GAME_H */

