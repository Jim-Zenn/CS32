#ifndef ACTOR_H_
#define ACTOR_H_

#include "GameConstants.h"
#include "GraphObject.h"

class StudentWorld;
class Penelope;

class Actor : public GraphObject {
public:
  Actor(StudentWorld *world, int imageID, double x, double y, Direction dir,
        int depth);
  virtual ~Actor(){};
  StudentWorld *world() const { return m_world; }
  Penelope *player() const;

  // Actions
  virtual void doSomething() {
    m_turnFinished = false;
    if (willBeRemoved())
      finishTurn();
  }
  virtual void die(){};
  virtual void infect(){};
  virtual void evacuate(){};
  virtual bool isHuman() const { return false; }
  virtual bool isZombie() const { return false; }
  virtual bool blocks() const { return false; }
  virtual bool isFlammable() const { return true; }
  bool willBeRemoved() const { return m_willBeRemoved; }
  bool turnFinished() const { return m_turnFinished; }

  double overlaps(const Actor *obj) const;
  double distTo(const Actor *obj) const;
  bool isNear(const Actor *obj) const;

  double nextNSpriteX(const Direction &dir, const int &n) const;
  double nextNSpriteY(const Direction &dir, const int &n) const;

  void finishTurn() { m_turnFinished = true; }

  virtual void scheduleRemoval() {
    finishTurn();
    m_willBeRemoved = true;
  }

private:
  StudentWorld *m_world;
  bool m_turnFinished = false;
  bool m_willBeRemoved = false;
};

class Wall : public Actor {
public:
  Wall(StudentWorld *world, double x, double y);
  virtual bool blocks() const { return true; }
  virtual bool isFlammable() const { return false; }
  virtual void scheduleRemoval() {}
};

class Agent : public Actor {
public:
  Agent(StudentWorld *world, int imageID, double x, double y,
        const double &speed);

  Direction getDirectionTowards(const Actor *target);
  virtual bool blocks() const { return true; }
  double speed() const { return m_speed; }
  bool isParalyzed() const { return m_isParalyzed; }
  bool isBlockedAtDir(const Direction &dir) const;

  double nextX(const Direction &dir) const {
    switch (dir) {
    case right:
      return getX() + speed();
    case left:
      return getX() - speed();
    default:
      return getX();
    }
  }
  double nextY(const Direction &dir) const {
    switch (dir) {
    case up:
      return getY() + speed();
    case down:
      return getY() - speed();
    default:
      return getY();
    }
  }
  void moveTowards(const Direction &dir) { moveTo(nextX(dir), nextY(dir)); }
  void toggleParalyzed() { m_isParalyzed = !m_isParalyzed; }

private:
  double m_speed;
  bool m_isParalyzed;
};

class Human : public Agent {
public:
  Human(StudentWorld *world, int imageID, double x, double y,
        const double &speed);
  virtual ~Human();
  virtual void doSomething() {
    Agent::doSomething();
    if (!turnFinished())
      updateInfection();
  }
  virtual bool isHuman() const { return true; }
  virtual void infect() {
    if (!isInfected())
      m_infectedTime = 0; // start counting
  }
  void updateInfection();

  bool isInfected() const { return m_infectedTime >= 0; }
  int infectionTime() const { return isInfected() ? m_infectedTime : 0; }
  virtual void cure() { m_infectedTime = -1; }
  virtual void mutate() = 0;

private:
  int m_infectedTime = -1;
};

class Penelope : public Human {
public:
  Penelope(StudentWorld *world, double x, double y);
  virtual void doSomething();
  virtual void mutate();
  virtual void die();
  virtual void evacuate();
  int gasCanCount() const { return m_nGasCan; }
  int landmineCount() const { return m_nLandmine; }
  int vaccineCount() const { return m_nVaccine; }
  void pickupGoodie();
  void pickupGasCan() {
    pickupGoodie();
    m_nGasCan += 5;
  }
  void pickupLandmine() {
    pickupGoodie();
    m_nLandmine += 2;
  }
  void pickupVaccine() {
    pickupGoodie();
    m_nVaccine++;
  }
  void useGasCan();
  void useLandmine();
  void useVaccine();

private:
  int m_nGasCan = 0;
  int m_nLandmine = 0;
  int m_nVaccine = 0;
};

class Citizen : public Human {
public:
  Citizen(StudentWorld *world, double x, double y);
  virtual void doSomething();
  virtual void die();
  virtual void evacuate();
  virtual void mutate();
  virtual void infect();
};

class Zombie : public Agent {
public:
  Zombie(StudentWorld *world, double x, double y);
  bool vomit();
  virtual void doSomething();
  virtual void die();
  virtual bool isZombie() const { return true; }
  int getPlan() const { return m_plan; }
  void setPlan(const int &plan) { m_plan = plan; }
  void decPlan() { m_plan--; }

private:
  int m_plan = 0;
};

class DumbZombie : public Zombie {
public:
  DumbZombie(StudentWorld *world, double x, double y) : Zombie(world, x, y) {}
  virtual void doSomething();
  virtual void die();
};

class SmartZombie : public Zombie {
public:
  SmartZombie(StudentWorld *world, double x, double y) : Zombie(world, x, y) {}
  virtual void doSomething();
  virtual void die();
};

class Goodie : public Actor {
public:
  Goodie(StudentWorld *world, int imageID, double x, double y)
      : Actor(world, imageID, x, y, right, 1) {}
  virtual void doSomething();
  virtual void die() { scheduleRemoval(); }
};

class GasCanGoodie : public Goodie {
public:
  GasCanGoodie(StudentWorld *world, double x, double y)
      : Goodie(world, IID_GAS_CAN_GOODIE, x, y) {}
  virtual void doSomething();
};

class LandmineGoodie : public Goodie {
public:
  LandmineGoodie(StudentWorld *world, double x, double y)
      : Goodie(world, IID_LANDMINE_GOODIE, x, y) {}
  virtual void doSomething();
};

class VaccineGoodie : public Goodie {
public:
  VaccineGoodie(StudentWorld *world, double x, double y)
      : Goodie(world, IID_VACCINE_GOODIE, x, y) {}
  virtual void doSomething();
};

class Projectile : public Actor {
public:
  Projectile(StudentWorld *world, int imageID, double x, double y,
             const Direction &dir)
      : Actor(world, imageID, x, y, dir, 0) {}
  virtual void doSomething();

private:
  int m_age = 0;
};

class Vomit : public Projectile {
public:
  Vomit(StudentWorld *world, double x, double y, const Direction &dir)
      : Projectile(world, IID_VOMIT, x, y, dir) {}
  virtual void doSomething();
};

class Flame : public Projectile {
public:
  Flame(StudentWorld *world, double x, double y, const Direction &dir)
      : Projectile(world, IID_FLAME, x, y, dir) {}
  virtual void doSomething();
};

class Landmine : public Actor {
public:
  Landmine(StudentWorld *world, double x, double y);
  virtual void doSomething();
  virtual void die();
  void explode();

private:
  bool isActive() { return m_remainingSafetyTicks == 0; }
  void decSafetyTicks() {
    if (!isActive())
      m_remainingSafetyTicks -= 1;
  }
  int m_remainingSafetyTicks = 30;
};

class Pit : public Actor {
public:
  Pit(StudentWorld *world, double x, double y)
      : Actor(world, IID_PIT, x, y, right, 0) {}
  virtual void doSomething();
  virtual void scheduleRemoval() {}
};

class Exit : public Actor {
public:
  Exit(StudentWorld *world, double x, double y)
      : Actor(world, IID_EXIT, x, y, right, 1) {}
  virtual void doSomething();
  virtual bool isFlammable() const { return false; }
  virtual void scheduleRemoval() {}
};

#endif // ACTOR_H_
