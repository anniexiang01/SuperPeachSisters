#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp

class Actor: public GraphObject
{
public:
    Actor(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething() = 0;
    bool isAlive();
    int power;
private:
    bool m_status;
    StudentWorld* m_world;
};

class Peach: public Actor
{
public:
    Peach(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    bool ifStar();
    bool ifShoot();
    bool ifJump();
    bool ifFlag();
    bool ifMario();
    
private:
    int m_health;
    bool starPower;
    bool shootPower;
    bool jumpPower;
    bool m_invincible;
    bool gotFlag;
    bool gotMario;
};

#endif // ACTOR_H_
