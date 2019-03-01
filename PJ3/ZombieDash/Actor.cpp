#include "Actor.h"
#include "StudentWorld.h"

#include <random>

Actor::Actor(StudentWorld * world, int imageID, double x, double y,
             Direction dir, int depth)
    : GraphObject(imageID, x, y, dir, depth), m_world(world) {}

double Actor::offsetX(const Direction &dir, const double &dist) const {
  switch (dir) {
  case LEFT:
    return getX() - dist;
  case RIGHT:
    return getX() + dist;
  default:
    return getX();
  }
}

double Actor::offsetY(const Direction &dir, const double &dist) const {
  switch (dir) {
  case UP:
    return getY() + dist;
  case DOWN:
    return getY() - dist;
  default:
    return getY();
  }
}

Penelope * Actor::player() const { return world()->player(); }

double Actor::overlaps(const Actor *obj) const {
  return world()->checkOverlap(this, obj);
}

double Actor::distTo(const Actor *obj) const {
  return world()->distBetween(this, obj);
}

bool Actor::isNear(const Actor *obj) const {
  return world()->checkNear(this, obj);
}

void Landmine::doSomething() {
  Actor::doSomething();
  if (!turnFinished() && !isActive()) {
    decSafetyTicks();
    finishTurn();
  }
  if (!turnFinished() && world()->checkOverlapWithAgent(getX(), getY())) {
    // Ready for the big boom-boom?
    explode();
  }
}

void Landmine::die() { explode(); }

void Landmine::explode() {
  scheduleRemoval();
  world()->playSound(SOUND_LANDMINE_EXPLODE);
  world()->addFlame(getX(), getY(), UP);

  double flameX, flameY;
  // north
  flameX = offsetX(UP, SPRITE_WIDTH);
  flameY = offsetY(UP, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // northeast
  flameX = offsetX(RIGHT, SPRITE_WIDTH);
  flameY = offsetY(UP, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // east
  flameX = offsetX(RIGHT, SPRITE_WIDTH);
  flameY = offsetY(RIGHT, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // southeast
  flameX = offsetX(RIGHT, SPRITE_WIDTH);
  flameY = offsetY(DOWN, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // south
  flameX = offsetX(DOWN, SPRITE_WIDTH);
  flameY = offsetY(DOWN, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // southwest
  flameX = offsetX(LEFT, SPRITE_WIDTH);
  flameY = offsetY(DOWN, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // west
  flameX = offsetX(LEFT, SPRITE_WIDTH);
  flameY = offsetY(LEFT, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);
  // northwest
  flameX = offsetX(LEFT, SPRITE_WIDTH);
  flameY = offsetY(UP, SPRITE_HEIGHT);
  world()->addFlame(flameX, flameY, UP);

  world()->addPit(getX(), getY());
}

Wall::Wall(StudentWorld *world, double x, double y)
    : Actor(world, IID_WALL, x, y, RIGHT, 0) {}

Landmine::Landmine(StudentWorld *world, double x, double y)
    : Actor(world, IID_LANDMINE, x, y, RIGHT, 1) {}

void Exit::doSomething() {
  // I AM THE DOOR. If anyone enters by Me, he will be saved.
  // (John 10:9-16 NKJV)
  Actor::doSomething();
  if (!turnFinished()) {
    world()->evacuateAt(getX(), getY());
  }
}

void Pit::doSomething() {
  Actor::doSomething();
  if (!turnFinished()) {
    world()->killAt(getX(), getY());
  }
}

Agent::Agent(StudentWorld *world, int imageID, double x, double y,
             const double &speed)
    : Actor(world, imageID, x, y, RIGHT, 0), m_speed(speed) {}

bool Agent::isBlockedAtDir(const Direction &dir) const {
  return world()->checkBlockAtDir(this, dir, speed());
}

Direction Agent::getDirectionTowards(const Actor *target) const {
  Direction dir = 0;
if (getX() == target->getX()) {
    if (target->getY() > getY())
        dir = UP;
    if (target->getY() < getY())
        dir = DOWN;
}
else if (getY() == target->getY()) {
    if (getX() < target->getX())
        dir = RIGHT;
    if (getX() > target->getX())
        dir = LEFT;
}
else {
    Direction dirX = getX() > target->getX() ? LEFT : RIGHT;
    Direction dirY = getY() > target->getY() ? DOWN : UP;
    bool isBlockedX = isBlockedAtDir(dirX);
    bool isBlockedY = isBlockedAtDir(dirY);
    if (!isBlockedX && isBlockedY)
      dir = dirX;
    else if (isBlockedX && !isBlockedY)
      dir = dirY;
    else
      dir = world()->bernoulliRandomBool(0.5) ? dirX : dirY;
  }
  return dir;
}

Human::Human(StudentWorld *world, int imageID, double x, double y,
             const double &speed)
    : Agent(world, imageID, x, y, speed) {
}

void Human::updateInfection() {
  if (!isInfected())
    return;
  m_infectedTime++;
  if (infectionTime() >= TIME_INFECTION_LIMIT)
    mutate();
}

Penelope::Penelope(StudentWorld *world, double x, double y)
    : Human(world, IID_PLAYER, x, y, SPEED_PENELOPE) {}

void Penelope::doSomething() {
  Human::doSomething();
  if (!turnFinished()) {
    // since penelope is still alive, let's see what she's gonna do
    int ch;
    bool keyhit = world()->getKey(ch);
    if (keyhit) {
      switch (ch) {
      case KEY_PRESS_SPACE:
        useGasCan();
        break;
      case KEY_PRESS_TAB:
        useLandmine();
        break;
      case KEY_PRESS_ENTER:
        useVaccine();
        break;
      case KEY_PRESS_RIGHT:
        setDirection(RIGHT);
        if (!isBlockedAtDir(RIGHT))
          moveTowards(RIGHT);
        break;
      case KEY_PRESS_UP:
        setDirection(UP);
        if (!isBlockedAtDir(UP))
          moveTowards(UP);
        break;
      case KEY_PRESS_LEFT:
        setDirection(LEFT);
        if (!isBlockedAtDir(LEFT))
          moveTowards(LEFT);
        break;
      case KEY_PRESS_DOWN:
        setDirection(DOWN);
        if (!isBlockedAtDir(DOWN))
          moveTowards(DOWN);
        break;
      default:;
      }
    }
  }
}

void Penelope::pickupGoodie() {
  world()->increaseScore(SCORE_GOT_GOODIE);
  world()->playSound(SOUND_GOT_GOODIE);
}

void Penelope::die() {
  scheduleRemoval();
  world()->playSound(SOUND_PLAYER_DIE);
}

void Penelope::mutate() { die(); }

void Penelope::evacuate() {
  if (world()->citizenCount() == 0)
    world()->finishLevel();
}

void Penelope::useLandmine() {
  if (landmineCount() > 0) {
    m_nLandmine--;
    world()->addLandmine(getX(), getY());
  }
}

void Penelope::useGasCan() {
  if (gasCanCount() > 0) {
    m_nGasCan--;
    world()->playSound(SOUND_PLAYER_FIRE);
    for (int i = 1; i <= FLAMETHROWER_RANGE; i++) {
      double flameX = offsetX(getDirection(), i * SPRITE_WIDTH);
      double flameY = offsetY(getDirection(), i * SPRITE_HEIGHT);
      if (!world()->addFlame(flameX, flameY, getDirection()))
        break;
    }
  }
}

void Penelope::useVaccine() {
  if (vaccineCount() > 0 && isInfected()) {
    cure();
    m_nVaccine--;
  }
}

Citizen::Citizen(StudentWorld *world, double x, double y)
    : Human(world, IID_CITIZEN, x, y, SPEED_CITIZEN) {}

void Citizen::doSomething() {
  Human::doSomething();
  if (!turnFinished()) {
    toggleParalyzed();
    if (isParalyzed())
      finishTurn();
  }
  double distToPenelope, distToZombie;
  distToPenelope = distTo(player());
  Actor *nearestZombie = world()->getNearestZombie(getX(), getY());
  if (nearestZombie != nullptr) {
    distToZombie = distTo(nearestZombie);
  } else {
    distToZombie = VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT;
  }
  if (!turnFinished() && distToPenelope <= distToZombie && isNear(player())) {
    Direction potentialDir = getDirectionTowards(player());
    if (!isBlockedAtDir(potentialDir)) {
      setDirection(potentialDir);
      moveTowards(potentialDir);
      finishTurn();
    }
  }
  if (!turnFinished() && distToPenelope > distToZombie &&
      isNear(nearestZombie)) {
    double nextDist[4];
    for (Direction dir = right; dir <= down; dir += 90) {
      Actor *nextNearestZombie =
          world()->getNearestZombie(nextX(dir), nextY(dir));
      nextDist[dir / 90] =
          isBlockedAtDir(dir)
              ? 0
              : world()->distBetween(nextNearestZombie, nextX(dir), nextY(dir));
    }
    int maxIdx = 0, maxNextDist = 0;
    for (int i = 0; i < 4; i++) {
      if (nextDist[i] > maxNextDist) {
        maxIdx = i;
        maxNextDist = nextDist[i];
      }
    }
    if (maxNextDist > distTo(nearestZombie)) {
      setDirection(maxIdx * 90);
      moveTowards(maxIdx * 90);
    }
  }
}

void Citizen::die() {
  scheduleRemoval();
  world()->decCitizenCount();
  world()->playSound(SOUND_CITIZEN_DIE);
  world()->increaseScore(SCORE_CITIZEN_DIE);
}

void Citizen::evacuate() {
  scheduleRemoval();
  world()->playSound(SOUND_CITIZEN_SAVED);
  world()->increaseScore(SCORE_CITIZEN_SAVED);
}

void Citizen::infect() {
  if (isInfected())
    return;
  world()->playSound(SOUND_CITIZEN_INFECTED);
  Human::infect();
}

void Citizen::mutate() {
  scheduleRemoval();
  world()->playSound(SOUND_ZOMBIE_BORN);
  world()->increaseScore(SCORE_CITIZEN_DIE);
    if (world()->bernoulliRandomBool(.7))
        world()->addDumbZombie(getX(), getY());
    else
        world()->addSmartZombie(getX(), getY());
}

Zombie::Zombie(StudentWorld *world, double x, double y)
    : Agent(world, IID_ZOMBIE, x, y, SPEED_ZOMBIE) {}

void Zombie::doSomething() {
  Agent::doSomething();
  if (!turnFinished()) {
    toggleParalyzed();
    if (isParalyzed())
      finishTurn();
  }
  if (!turnFinished()) {
    if (vomit())
      finishTurn();
  }
}

bool Zombie::vomit() {
  // see if any human is in the range
  double vomitX = offsetX(getDirection(), 1);
  double vomitY = offsetY(getDirection(), 1);
  if (world()->checkOverlapWithHuman(vomitX, vomitY)) {
    // there is a 1/3 chance that the zombie will vomit
    if (world()->unifRandomInt(1, 3) == 1) {
      world()->addVomit(vomitX, vomitY, getDirection());
      world()->playSound(SOUND_ZOMBIE_VOMIT);
      return true;
    }
  }
  return false;
}

void Zombie::die() {
  scheduleRemoval();
  world()->playSound(SOUND_ZOMBIE_DIE);
}

void SmartZombie::doSomething() {
  Zombie::doSomething();
  if (!turnFinished() && getPlan() == 0) {
    setPlan(world()->unifRandomInt(ZOMBIE_MOVEMENT_RANGE_MIN,
                                   ZOMBIE_MOVEMENT_RANGE_MAX));
    Actor *nearestHuman = world()->getNearestHuman(getX(), getY());
    if (nearestHuman != nullptr && isNear(nearestHuman)) {
      setDirection(getDirectionTowards(nearestHuman));
    } else {
      setDirection(world()->randomDirection());
    }
  }
  if (!turnFinished()) {
    if (!isBlockedAtDir(getDirection())) {
      moveTowards(getDirection());
      decPlan();
    } else {
      setPlan(0);
    }
  }
}

void DumbZombie::doSomething() {
  Zombie::doSomething();
  if (!turnFinished() && getPlan() == 0) {
    setPlan(world()->unifRandomInt(ZOMBIE_MOVEMENT_RANGE_MIN,
                                   ZOMBIE_MOVEMENT_RANGE_MAX));
    setDirection(world()->randomDirection());
  }
  if (!turnFinished()) {
    if (!isBlockedAtDir(getDirection())) {
      moveTowards(getDirection());
      decPlan();
    } else {
      setPlan(0);
    }
  }
}

void SmartZombie::die() {
  Zombie::die();
  if (world()->unifRandomInt(1, 10) == 1) {
    Direction flingDir = world()->randomDirection();
    double vaccineX = offsetX(flingDir, SPRITE_WIDTH);
    double vaccineY = offsetY(flingDir, SPRITE_HEIGHT);
    world()->addVaccineGoodie(vaccineX, vaccineY);
  }
  world()->increaseScore(SCORE_SMART_ZOMBIE_DIE);
}

void DumbZombie::die() {
  Zombie::die();
  world()->increaseScore(SCORE_DUMB_ZOMBIE_DIE);
}

void Goodie::doSomething() { Actor::doSomething(); }

void GasCanGoodie::doSomething() {
  Goodie::doSomething();
  if (!turnFinished() && overlaps(player())) {
    player()->pickupGasCan();
    scheduleRemoval();
  }
}

void LandmineGoodie::doSomething() {
  Goodie::doSomething();
  if (!turnFinished() && overlaps(player())) {
    player()->pickupLandmine();
    scheduleRemoval();
  }
}

void VaccineGoodie::doSomething() {
  Goodie::doSomething();
  if (!turnFinished() && overlaps(player())) {
    player()->pickupVaccine();
    scheduleRemoval();
  }
}

void Projectile::doSomething() {
  Actor::doSomething();
  if (!turnFinished()) {
    m_age++;
    if (m_age >= PROJECTILE_LIFE)
      scheduleRemoval();
  }
}

void Vomit::doSomething() {
  Projectile::doSomething();
  if (!turnFinished()) {
    world()->infectAt(getX(), getY());
  }
}

void Flame::doSomething() {
  Projectile::doSomething();
  if (!turnFinished()) {
    world()->killAt(getX(), getY());
  }
}
