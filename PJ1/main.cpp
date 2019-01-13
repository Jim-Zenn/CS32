/*
 * main.cpp
 * Copyright (C) 2019 Jim Zenn <zenn@ucla.edu>
 *
 * Distributed under terms of the MIT license.
 */


#include <iostream>
#include <string>
#include <utility>
#include <cstdlib>

#include "globals.h"
#include "Zombie.h"
#include "Arena.h"
#include "Player.h"
#include "Game.h"

using namespace std;

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

