#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Level.h"
#include <string>
#include <vector>
using namespace std;

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class Actor;

class Peach;

class StudentWorld : public GameWorld
{
public:
  StudentWorld(std::string assetPath);
  virtual int init();
  virtual int move();
  virtual void cleanUp();
    bool isBlockingObject(int x, int y);
    bool isOverlap(Actor* a, Actor* b);
    bool isOverlap(Actor* a, int x, int y);
    bool isOverlapPeach(Actor* a);
    
    bool ifPeachStar();
    bool ifPeachSameLevel(Actor* a);
    bool ifPeachLeft(Actor* a);
    bool ifPeachInRange(Actor* a);
    
    void damagePeach();
    bool damageActorAt(int x, int y);
    
    void setStar();
    void setShoot();
    void setJump();
    
    void addMushroom(int x, int y);
    void addStar(int x, int y);
    void addFlower(int x, int y);
    
    void bonkActorAt(int x, int y);
    void ifOverlapPeachBonk();
    void bonkPeach();
    
    void nextLevel();
    void endGame();
    
    void addPiranhaFire(int x, int y, int dir);
    void addPeachFire(int x, int y, int dir);
    void addShell(int x, int y, int dir);

private:
    vector<Actor*> m_actor; //use vector to store pointers to all actors except Peach
    Peach* m_peach; //pointer to Peach
    bool flag;
    bool mario;
    bool shell;
    int shellX;
    int shellY;
    int shellDir;
    bool piranhaFire;
    int pfireX;
    int pfireY;
    int pfireDir;
};

#endif // STUDENTWORLD_H_
