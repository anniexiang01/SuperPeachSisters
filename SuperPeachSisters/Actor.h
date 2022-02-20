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
    
    void setStar(int ticks);
    void setShoot();
    void setJump();
    void setHealth(int hit);
    
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

class Block: public Actor
{
public:
    Block(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
    void dropItem();
    int getItem();
private:
    int m_item;
};

class mushroomBlock: public Block
{
public:
    mushroomBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
};

class flowerBlock: public Block
{
public:
    flowerBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
};

class starBlock: public Block
{
public:
    starBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
};

class Pipe: public Actor
{
public:
    Pipe(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
};

class Goodie: public Actor
{
public:
    Goodie(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
private:
    virtual void setPower() = 0;
};

class Star: public Goodie
{
public:
    Star(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
};

class Mushroom: public Goodie
{
public:
    Mushroom(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
};

class Flower: public Goodie
{
public:
    Flower(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
};



#endif // ACTOR_H_
