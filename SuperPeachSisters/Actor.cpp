#include "Actor.h"
#include "StudentWorld.h"
#include "GameConstants.h"

// Students:  Add code to this file, Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): GraphObject(imageID, startX, startY, startDirection, depth, size)
{
    m_status = true;
    m_world = swp;
}

bool Actor::isAlive()
{
    return m_status;
}

void Actor::setDead()
{
    m_status = false;
}

StudentWorld* Actor::getWorld()
{
    return m_world;
}

bool Actor::isBlockOrPipe()
{
    if (this->blocks())
        return true;
    else
        return false;
}

Peach::Peach(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{
    m_health = 1;
    remaining_star_invincible = 0;
    remaining_temp_invincible = 0;
    tempInvincible = false;
    starPower = false;
    shootPower = false;
    jumpPower = false;
    gotFlag = false;
    gotMario = false;
    time_to_recharge_before_next_fire = 0;
    remaining_jump_distance = 0;
}

void Peach::doSomething()
{
    if(!isAlive())
        return;
    
    //if star
    if(starPower)
        remaining_star_invincible--;
    if(remaining_star_invincible == 0)
        starPower = false;
    
    //if temp incinvible
    if (tempInvincible)
        remaining_temp_invincible--;
    if (remaining_temp_invincible == 0)
        tempInvincible = false;
    
    //if recharging
    if (time_to_recharge_before_next_fire > 0)
        time_to_recharge_before_next_fire--;
    
    //if overlap, getActor->bonk
    //jumping
    //falling
    //key
    
    int key;
    if (getWorld()->getKey(key))
    {
        switch (key)
        {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if (getWorld()->isBlockingObject(getX() - 4, getY()))
                    getWorld()->getActorAt(getX() - 4, getY())->bonk();
                else
                    moveTo(getX() - 4, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if (getWorld()->isBlockingObject(getX() + 4, getY()))
                    getWorld()->getActorAt(getX() + 4, getY())->bonk();
                else
                    moveTo(getX() + 4, getY());
                break;
            case KEY_PRESS_SPACE:
                //implement this
                break;
            case KEY_PRESS_UP:
                if (shootPower && time_to_recharge_before_next_fire == 0)
                {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    /*Determine the x,y position directly in front of Peach that is
                     4 pixels away in the direction she’s facing.
                     Introduce a new fireball object at this location into your
                     StudentWorld. The fireball must have its direction set to the
                     same direction that Peach was facing when she fired.*/
                }
                break;
                
        }
        
    }
}

void Peach::bonk()
{
    if (starPower == true || tempInvincible == true)
        return;
    else
    {
        m_health--;
        remaining_temp_invincible = 10;
        shootPower = false;
        jumpPower = false;
        if (m_health == 0)
        {
            setDead();
            return;
        }
        else
        {
            getWorld()->playSound(SOUND_PLAYER_HURT);
            return;
        }
    }
}

bool Peach::blocks()
{
    return false;
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

//actors use getWorld()->getPeach()->bonk()
