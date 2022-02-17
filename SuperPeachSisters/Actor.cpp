#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): GraphObject(imageID, startX, startY, startDirection, depth, size)
{
    m_status = true;
}

bool Actor::isAlive()
{
    return m_status;
}

Peach::Peach(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{
    m_health = 1;
    m_invincible = false;
    starPower = false;
    shootPower = false;
    jumpPower = false;
    gotFlag = false;
    gotMario = false;
}

void Peach::doSomething()
{
    
}

bool Peach::ifFlag()
{
    return gotFlag;
}

bool Peach::ifMario()
{
    return gotMario;
}

bool Peach::ifStar()
{
    return starPower;
}

bool Peach::ifShoot()
{
    return shootPower;
}

bool Peach::ifJump()
{
    return jumpPower;
}
