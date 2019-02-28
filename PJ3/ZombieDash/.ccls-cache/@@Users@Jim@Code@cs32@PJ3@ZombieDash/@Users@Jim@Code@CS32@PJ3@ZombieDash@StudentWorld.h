#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#define OVERLAP_DISTANCE  10
#define NEAR_DISTANCE     80

#include "GameWorld.h"
#include "GraphObject.h"
#include "Actor.h"
#include <string>
#include <list>

using namespace std;

class StudentWorld : public GameWorld
{
public:
    StudentWorld(string assetPath);
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    // getters
    int humanCount() const { return m_nHumans; }
    bool levelFinished() const { return m_levelFinished; }
    Penelope * player() const { return m_player; }

    int distBetween(const GraphObject * a, const GraphObject * b) const {
        return distBetween(a->getX(), a->getY(), b->getX(), b->getY());
    }
    int distBetween(const double &x, const double &y,
                    const GraphObject * obj) const {
        return distBetween(x, y, obj->getX(), obj->getY());
    }
    int distBetween(const double &x1, const double &y1,
                    const double &x2, const double &y2) const {
        double dx = x1 - x2;
        double dy = y1 - y2;
        return sqrt(dx * dx + dy * dy);
    }

    bool checkOverlap(const GraphObject * a, const GraphObject * b) const {
        return overlap(a->getX(), a->getY(), b->getX(), b->getY());
    }
    bool overlap(const double &x1, const double &y1,
                 const double &x2, const double &y2) const {
        return distBetween(x1, y1, x2, y2) <= OVERLAP_DISTANCE;
    }

    bool checkNear(const GraphObject * a, const GraphObject * b) const {
        return distBetween(a, b) <= NEAR_DISTANCE;
    }
    void infectAt(const Vomit * v);
    void evacuateAt(const Exit * e);
    void killAt(const Actor * p);

    bool checkBlockAtDir(const Agent * f, const Direction &dir,
                         const double &speed) const;
    
    bool checkOverlapHuman(const double &x, const double &y);
    // check if any human will be covered by the vomit

    Actor * findNearestZombie(const double &x, const double &y);

    void setPlayer(Penelope * player) { m_player = player; }
    void addActor(Actor * actor) { m_actors.push_back(actor); }
    void removeActor(Actor * actor) { m_actors.remove(actor); }
    void incHumanCount() { m_nHumans ++; }
    void decHumanCount() { m_nHumans --; }
    void finishLevel() {
        m_levelFinished = true;
        playSound(SOUND_LEVEL_FINISHED);
    }
private:
    bool squareOverlap(const double &x1, const double &y1,
                       const double &x2, const double &y2,
                       const double &side_x, const double &side_y) const;
    list<Actor *> m_actors;
    Penelope *m_player = nullptr;
    int m_nHumans = 0;  // actors that needs redemption, Penelope included
    bool m_levelFinished = false;
};

#endif // STUDENTWORLD_H_
