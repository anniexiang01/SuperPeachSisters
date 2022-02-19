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
    
    //if overlap, getActor->bonk  ////////////////////////////////////////////////////////////////////////////////////
    //jumping  ////////////////////////////////////////////////////////////////////////////////////
    //falling  ////////////////////////////////////////////////////////////////////////////////////
    
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
                //implement this  ////////////////////////////////////////////////////////////////////////////////////
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

void Peach::setStar(int ticks)
{
    starPower = true;
    remaining_star_invincible = ticks;
}

void Peach::setShoot()
{
    shootPower = true;
}

void Peach::setJump()
{
    jumpPower = true;
}

void Peach::setHealth(int hit)
{
    m_health = hit;
}

Block::Block(int goodie, StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{
    m_goodie = goodie;
}

void Block::doSomething(){}

bool Block::blocks()
{
    return true;
}

void Block::bonk()
{
    if (m_goodie == 0)
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
    }
    else
    {
        getWorld()->playSound(SOUND_POWERUP_APPEARS);
        //new goodie x, y+8 ////////////////////////////////////////////////////////////////////////////////////
        m_goodie = 0;
    }
}

Pipe::Pipe(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{}

void Pipe::doSomething(){}

bool Pipe::blocks()
{
    return true;
}

void Pipe::bonk(){}

Goodie::Goodie(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{}

void Goodie::doSomething()
{
    //if overlap with peach
    if (getWorld()->isOverlapPeach(this))
    {
        setPower();
        setDead();
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        return;
    }
    
    //if not overlap with peach
    else
    {
        if (!(getWorld()->isBlockingObject(getX(), getY() - 2)))
            moveTo(getX(), getY() - 2);
        if (getDirection() == 0)
        {
            if (getWorld()->isBlockingObject(getX() - 2, getY()))
            {
                setDirection(180);
                return;
            }
            else
                moveTo(getX() - 2, getY());
        }
        else
        {
            if (getWorld()->isBlockingObject(getX() + 2, getY()))
            {
                setDirection(0);
                return;
            }
            else
                moveTo(getX() + 2, getY());
        }
    }
}

void Goodie::bonk(){}

bool Goodie::blocks()
{
    return false;
}

Star::Star(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Goodie(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Star::setPower()
{
    getWorld()->setStar();
    return;
}

Mushroom::Mushroom(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Goodie(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Mushroom::setPower()
{
    getWorld()->setJump();
    return;
}

Flower::Flower(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Goodie(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Flower::setPower()
{
    getWorld()->setShoot();
    return;
}
