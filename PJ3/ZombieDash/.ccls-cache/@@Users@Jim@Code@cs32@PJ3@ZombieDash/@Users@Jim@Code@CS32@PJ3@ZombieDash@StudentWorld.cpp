#include "StudentWorld.h"
#include "GameConstants.h"
#include "Level.h"
#include "Actor.h"
#include <string>
#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

GameWorld *createStudentWorld(string assetPath)
{
    return new StudentWorld(assetPath);
}

StudentWorld::StudentWorld(string assetPath) : GameWorld(assetPath)
{
}

int StudentWorld::init()
{
    // reset everything
    m_levelFinished = false;
    m_nHumans = 0;
    // load level file according to getLevel()
    Level lev(assetPath());

    string levelFile;
    // create the desired file name string
    if (getLevel() < 10) {
        levelFile = "level0" + to_string(getLevel()) + ".txt";
    } else if (getLevel() < 100) {
        levelFile = "level" + to_string(getLevel()) + ".txt";
    } else {
        cerr << "You beat 99 levels? I mean... that is... something." << endl;
        exit(1);
    }
    Level::LoadResult result = lev.loadLevel(levelFile);
    if (result == Level::load_fail_file_not_found) {
        // level file does not exist, the player has finished the game and wins
        return GWSTATUS_PLAYER_WON;
    }
    else if (result == Level::load_fail_bad_format) {
        cerr << "Your level was improperly formatted" << endl;
        exit(1);
    }
    else if (result != Level::load_success)
        cerr << "Level loading failed." << endl;

    // level successfully loaded, now construct everything one by one
    for (int x = 0; x < LEVEL_WIDTH; x ++) {
        for (int y = 0; y < LEVEL_HEIGHT; y ++) {
            Level::MazeEntry ge = lev.getContentsOf(x, y);
            switch (ge) {
            case Level::empty:
                break;
            // actors
            case Level::player:
                setPlayer(new Penelope(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::citizen:
                addActor(new Citizen(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::smart_zombie:
                addActor(new SmartZombie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::dumb_zombie:
                addActor(new DumbZombie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            // goodies
            case Level::vaccine_goodie:
                addActor(new VaccineGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::gas_can_goodie:
                addActor(new GasCanGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::landmine_goodie:
                addActor(new LandMineGoodie(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            // others
            case Level::exit:
                addActor(new Exit(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::wall:
                addActor(new Wall(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            case Level::pit:
                addActor(new Pit(this, SPRITE_WIDTH * x, SPRITE_HEIGHT * y));
                break;
            }
        }
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    setGameStatText("Score: " + to_string(getScore()) + " " +
                    "Level: " + to_string(getLevel()) + " " +
                    "Lives: " + to_string(getLives()) + " " +
                    "Vacc:  " + to_string(player()->vaccineCount()) + " " +
                    "Flames: " + to_string(player()->gasCanCount()) + " " +
                    "Mines:  " + to_string(player()->landmineCount()) + " " +
                    "Infected: " + to_string(player()->infectionTime())
                    );
    player()->doSomething();
    if (player()->willBeRemoved())
        return GWSTATUS_PLAYER_DIED;
    for (auto actor : m_actors) {
        actor->doSomething();
    }
    // remove dead actors (R.I.P.)
    m_actors.remove_if([this](Actor * a){
        if (a->willBeRemoved()) {
            delete a;
            return true;
        }
        return false;
    });
    return levelFinished() ? GWSTATUS_FINISHED_LEVEL : GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    delete player();
    for (auto actor: m_actors)
        delete actor;
    m_actors.clear();
}

bool StudentWorld::checkBlockAtDir(const Agent * f, const Direction &dir,
                                   const double &speed) const {
    int x1, y1, x2, y2;
    x1 = f->getX();
    y1 = f->getY();
    if (dir == 0) x1 += speed;
    if (dir == 90) y1 += speed;
    if (dir == 180) x1 -= speed;
    if (dir == 270) y1 -= speed;
    for (auto actor : m_actors) {
        if (actor == f || ! actor->doesBlock()) continue;
        x2 = actor->getX();
        y2 = actor->getY();
        if (squareOverlap(x1, y1, x2, y2, SPRITE_WIDTH - 1, SPRITE_HEIGHT - 1))
            return true;
    }
    if (m_player != f) {
        x2 = m_player->getX();
        y2 = m_player->getY();
        if (squareOverlap(x1, y1, x2, y2, SPRITE_WIDTH - 1, SPRITE_HEIGHT - 1))
            return true;
    }
    return false;
}

bool StudentWorld::checkOverlapHuman(const double &x, const double &y) {
    for (auto actor : m_actors) {
        if (actor->isHuman() && overlap(x, y, actor->getX(), actor->getY()))
            return true;
    }
    if (overlap(x, y, m_player->getX(), m_player->getY()))
        return true;
    return false;
}

bool StudentWorld::squareOverlap(const double &x1, const double &y1,
                                 const double &x2, const double &y2,
                                 const double &side_x,
                                 const double &side_y) const {
    if (x2 >= x1 - side_x && x2 <= x1 + side_x) {
        if (y2 >= y1 - side_y && y2 <= y1 + side_y) {
            return true;
        }
    }
    if (y2 >= y1 - side_y && y2 <= y1 + side_y) {
        if (x2 >= x1 - side_x && x2 <= x1 + side_x) {
            return true;
        }
    }
    return false;
}

void StudentWorld::infectAt(const Vomit * v)
{
    for (auto actor : m_actors) {
        if (actor->isHuman() && checkOverlap(v, actor)) {
            // Knock-knock-knockin' on heaven's door
            actor->infect();
        }
    }
    if (checkOverlap(v, player()))
        player()->infect();
}

void StudentWorld::evacuateAt(const Exit * e)
{
    for (auto actor : m_actors) {
        if (actor->isHuman() && checkOverlap(e, actor)) {
            // Knock-knock-knockin' on heaven's door
            actor->evacuate();
        }
    }
    if (checkOverlap(e, player()) && humanCount() == 1) {
        cerr << "evaculate called" << endl;
        player()->evacuate();
    }
}

void StudentWorld::killAt(const Actor * p)
{
    for (auto actor : m_actors) {
        if (!actor->isPermanent() && checkOverlap(p, actor)) {
            actor->die();
        }
    }
    if (checkOverlap(p, player()))
        player()->die();
}

Actor * StudentWorld::findNearestZombie(const double &x, const double &y) {
    double minDistSquare = VIEW_WIDTH * VIEW_WIDTH + VIEW_HEIGHT * VIEW_HEIGHT;
    double currDistSquare;
    Actor * nearestZombie = nullptr;
    for (auto actor : m_actors) {
        if (!actor->isZombie()) continue;
        currDistSquare = distBetween(x, y, actor->getX(), actor->getY());
        if (currDistSquare < minDistSquare) {
            minDistSquare = currDistSquare;
            nearestZombie = actor;
        }
    }
    return nearestZombie;
}
