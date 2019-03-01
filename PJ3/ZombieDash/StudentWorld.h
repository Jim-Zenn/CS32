#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#define OVERLAP_DISTANCE 10
#define NEAR_DISTANCE 80

#define RIGHT 0
#define UP 90
#define LEFT 180
#define DOWN 270

#define SCORE_GOT_GOODIE 50
#define SCORE_CITIZEN_DIE -1000
#define SCORE_CITIZEN_SAVED 500
#define SCORE_DUMB_ZOMBIE_DIE 1000
#define SCORE_SMART_ZOMBIE_DIE 2000

#define SPEED_PENELOPE 4
#define SPEED_CITIZEN 2
#define SPEED_ZOMBIE 1

#define PROJECTILE_LIFE 2
#define FLAMETHROWER_RANGE 3
#define TIME_INFECTION_LIMIT 500
#define ZOMBIE_MOVEMENT_RANGE_MIN 3
#define ZOMBIE_MOVEMENT_RANGE_MAX 10

#include "GameWorld.h"
#include "GraphObject.h"

#include <list>

class Actor;
class Penelope;

using namespace std;

class StudentWorld : public GameWorld {
public:
  StudentWorld(string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();

  // Accessors
  int citizenCount() const { return m_nCitizen; }
  bool levelFinished() const { return m_levelFinished; }
  bool playerDied() const { return player()->willBeRemoved(); }
  Penelope *player() const { return m_player; }

  void setPlayer(Penelope *player) { m_player = player; }
  void addActor(Actor *actor) { m_actors.push_back(actor); }
  void removeActor(Actor *actor) { m_actors.remove(actor); }

  // Actor adders
  bool addPlayer(const double &x, const double &y);
  bool addCitizen(const double &x, const double &y);
  bool addSmartZombie(const double &x, const double &y);
  bool addDumbZombie(const double &x, const double &y);
  bool addVaccineGoodie(const double &x, const double &y);
  bool addGasCanGoodie(const double &x, const double &y);
  bool addLandmineGoodie(const double &x, const double &y);
  bool addExit(const double &x, const double &y);
  bool addWall(const double &x, const double &y);
  bool addPit(const double &x, const double &y);
  bool addLandmine(const double &x, const double &y);
  bool addFlame(const double &x, const double &y, const Direction &dir);
  bool addVomit(const double &x, const double &y, const Direction &dir);

  // Helpers
  bool squareOverlap(const double &x1, const double &y1, const double &x2,
                     const double &y2, const double &sideX,
                     const double &sideY) const;
  bool squareOverlap(const Actor *obj, const double &x2, const double &y2,
                     const double &width, const double &height) const;
  bool squareOverlap(const Actor *obj1, const Actor *obj2, const double &width,
                     const double &height) const;
  int distBetween(const double &x1, const double &y1, const double &x2,
                  const double &y2) const;
  int distBetween(const Actor *obj, const double &x, const double &y) const;
  int distBetween(const Actor *obj1, const Actor *obj2) const;
  bool checkOverlap(const double &x1, const double &y1, const double &x2,
                    const double &y2) const;
  bool checkOverlap(const Actor *obj, const double &x, const double &y) const;
  bool checkOverlap(const Actor *obj1, const Actor *obj2) const;
  bool checkOverlapWithHuman(const double &x, const double &y) const;
  bool checkOverlapWithAgent(const double &x, const double &y) const;
  bool checkNear(const Actor *obj1, const Actor *obj2) const {
    return distBetween(obj1, obj2) <= NEAR_DISTANCE;
  }
  bool checkFlammableAt(const double &x, const double &y) const;
  bool checkBlockAtDir(const Actor *obj, const Direction &dir,
                         const double &dist) const;

  // Actions
  void infectAt(const double &x, const double &y);
  void evacuateAt(const double &x, const double &y);
  void killAt(const double &x, const double &y);

  Actor *getNearestZombie(const double &x, const double &y) const;
  Actor *getNearestHuman(const double &x, const double &y) const;

  // Random Generators
  int unifRandomInt(const int &min, const int &max) const;
  bool bernoulliRandomBool(const double &p) const;
  Direction randomDirection() const { return unifRandomInt(0, 3) * 90; };

  void incCitizenCount() { m_nCitizen++; }
  void decCitizenCount() { m_nCitizen--; }
  void finishLevel() {
    m_levelFinished = true;
    playSound(SOUND_LEVEL_FINISHED);
  }

private:
  list<Actor *> m_actors;
  Penelope *m_player;
  int m_nCitizen = 0; // actors that need to be saved, Penelope included
  bool m_levelFinished = false;
};

#endif // STUDENTWORLD_H_
