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
    void setDead();
    StudentWorld* getWorld();
    int power;
    bool isBlockOrPipe();
    virtual bool blocks() = 0;
    virtual void bonk() = 0;
private:
    bool m_status;
    StudentWorld* m_world;
};

class Peach: public Actor
{
public:
    Peach(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
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
    int remaining_star_invincible; //in ticks
    int remaining_temp_invincible;
    bool tempInvincible;
    bool gotFlag;
    bool gotMario;
    int time_to_recharge_before_next_fire;
    int remaining_jump_distance;
};



#endif // ACTOR_H_
