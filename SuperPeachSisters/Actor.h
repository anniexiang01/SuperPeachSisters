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
    virtual bool blocks() = 0;
    virtual void bonk() = 0;
    virtual bool damage();
    virtual ~Actor();
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
    virtual bool damage();
    bool ifStar();
    bool ifShoot();
    bool ifJump();
    
    void setStar(int ticks);
    void setShoot();
    void setJump();
    void setHealth(int hit);
    
    virtual ~Peach();
    
private:
    int m_health;
    bool starPower;
    bool shootPower;
    bool jumpPower;
    int remaining_star_invincible; //in ticks
    int remaining_temp_invincible;
    bool tempInvincible;
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
    virtual ~Block();
private:
    int m_item;
};

class mushroomBlock: public Block
{
public:
    mushroomBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
    virtual ~mushroomBlock();
};

class flowerBlock: public Block
{
public:
    flowerBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
    virtual ~flowerBlock();
};

class starBlock: public Block
{
public:
    starBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void bonk();
    virtual ~starBlock();
};

class Pipe: public Block
{
public:
    Pipe(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual ~Pipe();
};

class Goodie: public Actor
{
public:
    Goodie(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
    virtual ~Goodie();
private:
    virtual void setPower() = 0;
};

class Star: public Goodie
{
public:
    Star(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
    virtual ~Star();
};

class Mushroom: public Goodie
{
public:
    Mushroom(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
    virtual ~Mushroom();
};

class Flower: public Goodie
{
public:
    Flower(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void setPower();
    virtual ~Flower();
};

class Portal: public Actor
{
public:
    Portal(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
    virtual ~Portal();
private:
    virtual void goPortal() = 0;
};

class Flag: public Portal
{
public:
    Flag(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void goPortal();
    virtual ~Flag();
};

class Mario: public Portal
{
public:
    Mario(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void goPortal();
    virtual ~Mario();
};

class Enemy: public Actor
{
public:
    Enemy(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
    virtual bool damage();
    virtual ~Enemy();
private:
    virtual void postDeath();
};

class Goomba: public Enemy
{
public:
    Goomba(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual ~Goomba();
};

class Koopa: public Enemy
{
public:
    Koopa(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    void postDeath();
    virtual ~Koopa();
};

class Piranha: public Enemy
{
public:
    Piranha(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual ~Piranha();
private:
    int firing_delay;
};

class Projectile: public Actor
{
public:
    Projectile(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual void doSomething();
    virtual bool blocks();
    virtual void bonk();
    virtual ~Projectile();
private:
    virtual void tryDamage();
};

class PiranhaFire: public Projectile
{
public:
    PiranhaFire(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    void tryDamage();
    virtual ~PiranhaFire();
};

class PeachFire: public Projectile
{
public:
    PeachFire(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual ~PeachFire();
};

class Shell: public Projectile
{
public:
    Shell(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size);
    virtual ~Shell();
};

#endif // ACTOR_H_
