#include "GameConstants.h"
#include "Level.h"

#include "Actor.h"
#include "StudentWorld.h"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <random>
#include <string>

using namespace std;

GameWorld *createStudentWorld(string assetPath) {
  return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath) {}

int StudentWorld::init() {
  // reset everything
  m_levelFinished = false;
  m_nCitizen = 0;
  // load level file according to getLevel()
  Level level(assetPath());

  string levelFile;
  // create the desired file name string
  if (getLevel() < 10) {
    levelFile = "level0" + to_string(getLevel()) + ".txt";
  } else if (getLevel() < 100) {
    levelFile = "level" + to_string(getLevel()) + ".txt";
  } else {
    // "You beat 99 levels? You deserve an award."
    return GWSTATUS_PLAYER_WON;
  }
  Level::LoadResult result = level.loadLevel(levelFile);
  if (result == Level::load_fail_file_not_found) {
    // level file does not exist, the player has finished the game and wins
    return GWSTATUS_PLAYER_WON;
  } else if (result == Level::load_fail_bad_format) {
    cerr << "Your level was improperly formatted" << endl;
    exit(1);
  } else if (result != Level::load_success)
    cerr << "Level loading failed." << endl;

  // level successfully loaded, now construct everything one by one
  for (int x = 0; x < LEVEL_WIDTH; x++) {
    for (int y = 0; y < LEVEL_HEIGHT; y++) {
      Level::MazeEntry ge = level.getContentsOf(x, y);
      switch (ge) {
      case Level::empty:
        break;
      // actors
      case Level::player:
        addPlayer(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::citizen:
        addCitizen(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::smart_zombie:
        addSmartZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::dumb_zombie:
        addDumbZombie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      // goodies
      case Level::vaccine_goodie:
        addVaccineGoodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::gas_can_goodie:
        addGasCanGoodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::landmine_goodie:
        addLandmineGoodie(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      // others
      case Level::exit:
        addExit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::wall:
        addWall(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      case Level::pit:
        addPit(SPRITE_WIDTH * x, SPRITE_HEIGHT * y);
        break;
      }
    }
  }
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  setGameStatText("Score: " + to_string(getScore()) + "  " +
                  "Level: " + to_string(getLevel()) + "  " +
                  "Lives: " + to_string(getLives()) + "  " +
                  "Vacc:  " + to_string(player()->vaccineCount()) + "  " +
                  "Flames: " + to_string(player()->gasCanCount()) + "  " +
                  "Mines:  " + to_string(player()->landmineCount()) + "  " +
                  "Infected: " + to_string(player()->infectionTime()));
  if (playerDied()) {
    decLives();
    return GWSTATUS_PLAYER_DIED;
  }
  for (auto actor : m_actors)
    actor->doSomething();
  // remove dead actors (R.I.P.)
  m_actors.remove_if([this](Actor *a) {
    bool shouldRemove = a->willBeRemoved();
    if (shouldRemove)
      delete a;
    return shouldRemove;
  });

  return levelFinished() ? GWSTATUS_FINISHED_LEVEL : GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
  for (auto actor : m_actors)
    delete actor;
  m_actors.clear();
}

// Actor Adders

bool StudentWorld::addPlayer(const double &x, const double &y) {
  setPlayer(new Penelope(this, x, y));
  addActor(player());
  return true;
}
bool StudentWorld::addCitizen(const double &x, const double &y) {
  incCitizenCount();
  addActor(new Citizen(this, x, y));
  return true;
}

bool StudentWorld::addSmartZombie(const double &x, const double &y) {
  addActor(new SmartZombie(this, x, y));
  return true;
}

bool StudentWorld::addDumbZombie(const double &x, const double &y) {
  addActor(new DumbZombie(this, x, y));
  return true;
}

bool StudentWorld::addVaccineGoodie(const double &x, const double &y) {
  addActor(new VaccineGoodie(this, x, y));
  return true;
}

bool StudentWorld::addGasCanGoodie(const double &x, const double &y) {
  addActor(new GasCanGoodie(this, x, y));
  return true;
}

bool StudentWorld::addLandmineGoodie(const double &x, const double &y) {
  addActor(new LandmineGoodie(this, x, y));
  return true;
}

bool StudentWorld::addExit(const double &x, const double &y) {
  addActor(new Exit(this, x, y));
  return true;
}

bool StudentWorld::addWall(const double &x, const double &y) {
  addActor(new Wall(this, x, y));
  return true;
}

bool StudentWorld::addPit(const double &x, const double &y) {
  addActor(new Pit(this, x, y));
  return true;
}

bool StudentWorld::addLandmine(const double &x, const double &y) {
  addActor(new Landmine(this, x, y));
  return true;
}

bool StudentWorld::addFlame(const double &x, const double &y,
                            const Direction &dir) {
  bool isFlammable = checkFlammableAt(x, y);
  if (isFlammable)
    addActor(new Flame(this, x, y, dir));
  return isFlammable;
}

bool StudentWorld::addVomit(const double &x, const double &y,
                            const Direction &dir) {
  addActor(new Vomit(this, x, y, dir));
  return true;
}

// Helpers
int StudentWorld::distBetween(const double &x1, const double &y1,
                              const double &x2, const double &y2) const {
  double dx = x1 - x2;
  double dy = y1 - y2;
  return sqrt(dx * dx + dy * dy);
}

int StudentWorld::distBetween(const Actor *obj, const double &x,
                              const double &y) const {
  return distBetween(obj->getX(), obj->getY(), x, y);
}

int StudentWorld::distBetween(const Actor *obj1, const Actor *obj2) const {
  return distBetween(obj1, obj2->getX(), obj2->getY());
}

bool StudentWorld::checkOverlap(const double &x1, const double &y1,
                                const double &x2, const double &y2) const {
  return distBetween(x1, y1, x2, y2) <= OVERLAP_DISTANCE;
}

bool StudentWorld::checkOverlap(const Actor *obj, const double &x,
                                const double &y) const {
  return checkOverlap(obj->getX(), obj->getY(), x, y);
}

bool StudentWorld::checkOverlap(const Actor *obj1, const Actor *obj2) const {
  return checkOverlap(obj1, obj2->getX(), obj2->getY());
}

bool StudentWorld::checkFlammableAt(const double &x, const double &y) const {
  for (auto actor : m_actors) {
    if (!actor->isFlammable() && checkOverlap(actor, x, y))
      return false;
  }
  return true;
}

bool StudentWorld::checkBlockAtDir(const Actor *obj, const Direction &dir,
                                   const double &dist) const {
  int x1, y1, x2, y2;
  x1 = obj->getX();
  y1 = obj->getY();
  if (dir == RIGHT)
    x1 += dist;
  if (dir == UP)
    y1 += dist;
  if (dir == LEFT)
    x1 -= dist;
  if (dir == DOWN)
    y1 -= dist;
  for (auto actor : m_actors) {
    if (actor == obj || !actor->blocks())
      continue;
    x2 = actor->getX();
    y2 = actor->getY();
    if (squareOverlap(x1, y1, x2, y2, SPRITE_WIDTH - 1, SPRITE_HEIGHT - 1))
      return true;
  }
  return false;
}

bool StudentWorld::checkOverlapWithHuman(const double &x,
                                         const double &y) const {

  for (auto actor : m_actors) {
    if (actor->isHuman() && checkOverlap(actor, x, y)) {
      return true;
    }
  }
  return false;
}

bool StudentWorld::checkOverlapWithAgent(const double &x,
                                         const double &y) const {
  for (auto actor : m_actors) {
    if ((actor->isHuman() || actor->isZombie()) && checkOverlap(actor, x, y))
      return true;
  }
  return false;
}

bool StudentWorld::squareOverlap(const double &x1, const double &y1,
                                 const double &x2, const double &y2,
                                 const double &width,
                                 const double &height) const {
  if (x2 >= x1 - width && x2 <= x1 + width && y2 >= y1 - height &&
      y2 <= y1 + height) {
    return true;
  }
  if (y2 >= y1 - height && y2 <= y1 + height && x2 >= x1 - width &&
      x2 <= x1 + width) {
    return true;
  }
  return false;
}

bool StudentWorld::squareOverlap(const Actor *obj1, const double &x2,
                                 const double &y2, const double &width,
                                 const double &height) const {
  return squareOverlap(obj1->getX(), obj1->getY(), x2, y2, width, height);
}

bool StudentWorld::squareOverlap(const Actor *obj1, const Actor *obj2,
                                 const double &width,
                                 const double &height) const {
  return squareOverlap(obj1, obj2->getX(), obj2->getY(), width, height);
}

void StudentWorld::infectAt(const double &x, const double &y) {
  for (auto actor : m_actors) {
    if (!actor->willBeRemoved() && checkOverlap(actor, x, y))
      actor->infect();
  }
}

void StudentWorld::evacuateAt(const double &x, const double &y) {
  for (auto actor : m_actors) {
    if (!actor->willBeRemoved() && checkOverlap(actor, x, y))
      actor->evacuate();
  }
}

void StudentWorld::killAt(const double &x, const double &y) {
  for (auto actor : m_actors) {
    if (!actor->willBeRemoved() && checkOverlap(actor, x, y))
      // Knock-knock-knockin' on heaven's door
      actor->die();
  }
}

Actor *StudentWorld::getNearestZombie(const double &x, const double &y) const {
  double minDist = sqrt(VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT);
  double currDist;
  Actor *nearestZombie = nullptr;
  for (auto actor : m_actors) {
    if (!actor->isZombie())
      continue;
    currDist = distBetween(actor, x, y);
    if (currDist < minDist) {
      minDist = currDist;
      nearestZombie = actor;
    }
  }
  return nearestZombie;
}

Actor *StudentWorld::getNearestHuman(const double &x, const double &y) const {
  double minDist = sqrt(VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT);
  double currDist;
  Actor *nearestHuman = nullptr;
  for (auto actor : m_actors) {
    if (!actor->isHuman())
      continue;
    currDist = distBetween(actor, x, y);
    if (currDist < minDist) {
      minDist = currDist;
      nearestHuman = actor;
    }
  }
  return nearestHuman;
}

int StudentWorld::unifRandomInt(const int &min, const int &max) const {
  random_device rd;
  uniform_int_distribution<int> dist(min, max);
  return dist(rd);
}

bool StudentWorld::bernoulliRandomBool(const double &p) const {
  random_device rd;
  bernoulli_distribution dist(p);
  return dist(rd);
}
