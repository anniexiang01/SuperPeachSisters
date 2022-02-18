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
    Peach* getPeach();
    bool isBlockingObject(int x, int y);
    Actor* getActorAt(int x, int y);

private:
    vector<Actor*> m_actor; //use vector to store pointers to all actors except Peach
    Peach* m_peach; //pointer to Peach
};

#endif // STUDENTWORLD_H_
