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

using namespace std;

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function declarations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir);

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

///////////////////////////////////////////////////////////////////////////
//  Player implementations
///////////////////////////////////////////////////////////////////////////

Player::Player(Arena* ap, int r, int c)
{
    if (ap == nullptr)
    {
        cout << "***** The player must be created in some Arena!" << endl;
        exit(1);
    }
    if (r < 1  ||  r > ap->rows()  ||  c < 1  ||  c > ap->cols())
    {
        cout << "**** Player created with invalid coordinates (" << r
             << "," << c << ")!" << endl;
        exit(1);
    }
    m_arena = ap;
    m_row = r;
    m_col = c;
    m_age = 0;
    m_dead = false;
}

int Player::row() const
{
    return m_row;
}

int Player::col() const
{
    return m_col;
}

int Player::age() const
{
    return m_age;
}

void Player::stand()
{
    m_age++;
}

void Player::moveOrAttack(int dir)
{
    m_age++;
    int r = m_row;
    int c = m_col;
    if (m_arena->determineNewPosition(r, c, dir))
    {
        if (m_arena->numZombiesAt(r, c) > 0)
            m_arena->attackZombieAt(r, c, dir);
        else
        {
            m_row = r;
            m_col = c;
        }
    }
}

bool Player::isDead() const
{
    return m_dead;
}

void Player::setDead()
{
    m_dead = true;
}

///////////////////////////////////////////////////////////////////////////
//  Game implementations
///////////////////////////////////////////////////////////////////////////

Game::Game(int rows, int cols, int nZombies)
{
    if (nZombies < 0)
    {
        cout << "***** Cannot create Game with negative number of zombies!" << endl;
        exit(1);
    }
    if (nZombies > MAXZOMBIES)
    {
        cout << "***** Trying to create Game with " << nZombies
             << " zombies; only " << MAXZOMBIES << " are allowed!" << endl;
        exit(1);
    }
    if (rows == 1  &&  cols == 1  &&  nZombies > 0)
    {
        cout << "***** Cannot create Game with nowhere to place the zombies!" << endl;
        exit(1);
    }

        // Create arena
    m_arena = new Arena(rows, cols);

        // Add player
    int rPlayer = randInt(1, rows);
    int cPlayer = randInt(1, cols);
    m_arena->addPlayer(rPlayer, cPlayer);

      // Populate with zombies
    while (nZombies > 0)
    {
        int r = randInt(1, rows);
        int c = randInt(1, cols);
          // Don't put a zombie where the player is
        if (r == rPlayer  &&  c == cPlayer)
            continue;
        m_arena->addZombie(r, c);
        nZombies--;
    }
}

Game::~Game()
{
    delete m_arena;
}

void Game::play()
{
    m_arena->display();
    Player* p = m_arena->player();
    if (p == nullptr)
        return;
    while ( ! m_arena->player()->isDead()  &&  m_arena->zombieCount() > 0)
    {
        cout << endl;
        cout << "Move (u/d/l/r//q): ";
        string action;
        getline(cin,action);
        if (action.size() == 0)  // player stands
            p->stand();
        else
        {
            switch (action[0])
            {
              default:   // if bad move, nobody moves
                cout << '\a' << endl;  // beep
                continue;
              case 'q':
                return;
              case 'u':
              case 'd':
              case 'l':
              case 'r':
                p->moveOrAttack(decodeDirection(action[0]));
                break;
            }
        }
        m_arena->moveZombies();
        m_arena->display();
    }
}

///////////////////////////////////////////////////////////////////////////
//  Auxiliary function implementations
///////////////////////////////////////////////////////////////////////////

int decodeDirection(char dir)
{
    switch (dir)
    {
      case 'u':  return UP;
      case 'd':  return DOWN;
      case 'l':  return LEFT;
      case 'r':  return RIGHT;
    }
    return -1;  // bad argument passed in!
}

///////////////////////////////////////////////////////////////////////////
//  main()
///////////////////////////////////////////////////////////////////////////

int main()
{
      // Create a game
      // Use this instead to create a mini-game:   Game g(3, 4, 2);
    Game g(7, 8, 25);

      // Play the game
    g.play();
}

