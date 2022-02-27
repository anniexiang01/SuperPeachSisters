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

bool Actor::damage()
{
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
    
    //if overlap, bonkActor at
    getWorld()->ifOverlapPeachBonk();
    
    //jumping
    if (remaining_jump_distance > 0)
    {
        if (getWorld()->isBlockingObject(getX(), getY() + 4))
        {
            getWorld()->bonkActorAt(getX(), getY() + 4);
            remaining_jump_distance = 0;
        }
        else
        {
            moveTo(getX(), getY() + 4);
            remaining_jump_distance--;
        }
    }
    
    //falling
    else
    {
        if (!(getWorld()->isBlockingObject(getX(), getY()) || getWorld()->isBlockingObject(getX(), getY() - 1 ) || getWorld()->isBlockingObject(getX(), getY() - 2) || getWorld()->isBlockingObject(getX(), getY() - 3)))
            moveTo(getX(), getY() - 4);
    }
    
    
    //key
    int key;
    if (getWorld()->getKey(key))
    {
        switch (key)
        {
            case KEY_PRESS_LEFT:
                setDirection(180);
                if (getWorld()->isBlockingObject(getX() - 4, getY()))
                    getWorld()->bonkActorAt(getX() - 4, getY());
                else
                    moveTo(getX() - 4, getY());
                break;
            case KEY_PRESS_RIGHT:
                setDirection(0);
                if (getWorld()->isBlockingObject(getX() + 4, getY()))
                    getWorld()->bonkActorAt(getX() + 4, getY());
                else
                    moveTo(getX() + 4, getY());
                break;
            case KEY_PRESS_SPACE:
                if (shootPower && time_to_recharge_before_next_fire == 0)
                {
                    getWorld()->playSound(SOUND_PLAYER_FIRE);
                    time_to_recharge_before_next_fire = 8;
                    if (getDirection() == 180) //idk why peach is backwards lol so
                        getWorld()->addPeachFire(getX() - 4, getY(), 0);
                    else
                        getWorld()->addPeachFire(getX() + 4, getY(), 180);
                }
                break;
            case KEY_PRESS_UP:
                if (getWorld()->isBlockingObject(getX(), getY() - 1))
                {
                    if (jumpPower)
                        remaining_jump_distance = 12;
                    else
                        remaining_jump_distance = 8;
                    getWorld()->playSound(SOUND_PLAYER_JUMP);
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

bool Peach::damage()
{
    bonk();
    return true;
}

bool Peach::blocks()
{
    return false;
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

Block::Block(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{
    m_item = 1;
}

mushroomBlock::mushroomBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Block(swp, imageID, startX, startY, startDirection, depth, size)
{}

flowerBlock::flowerBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Block(swp, imageID, startX, startY, startDirection, depth, size)
{}

starBlock::starBlock(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Block(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Block::dropItem()
{
    m_item = 0;
}

int Block::getItem()
{
    return m_item;
}

void Block::doSomething(){}

bool Block::blocks()
{
    return true;
}

void Block::bonk()
{
    getWorld()->playSound(SOUND_PLAYER_BONK);
}

void mushroomBlock::bonk()
{
    if (getItem() == 1)
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->addMushroom(getX(), getY() + 8);
        dropItem();
    }
    else
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
    }
}

void flowerBlock::bonk()
{
    if (getItem() == 1)
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->addFlower(getX(), getY() + 8);
        dropItem();
    }
    else
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
    }
}

void starBlock::bonk()
{
    if (getItem() == 1)
    {
        getWorld()->playSound(SOUND_PLAYER_POWERUP);
        getWorld()->addStar(getX(), getY() + 8);
        dropItem();
    }
    else
    {
        getWorld()->playSound(SOUND_PLAYER_BONK);
    }
}

Pipe::Pipe(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Block(swp, imageID, startX, startY, startDirection, depth, size)
{}


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

Portal::Portal(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{}

void Portal::bonk(){}

bool Portal::blocks()
{
    return false;
}

void Portal::doSomething()
{
    if (!isAlive())
        return;
    
    if (getWorld()->isOverlapPeach(this))
    {
        goPortal();
        setDead();
    }
}

Flag::Flag(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Portal(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Flag::goPortal()
{
    getWorld()->nextLevel();
}

Mario::Mario(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Portal(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Mario::goPortal()
{
    getWorld()->endGame();
}

Enemy::Enemy(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{}

bool Enemy::blocks()
{
    return false;
}

void Enemy::doSomething()
{
    if (!isAlive())
        return;
    
    if (getWorld()->isOverlapPeach(this))
    {
        getWorld()->bonkPeach();
        return;
    }
    
    else
    {
        if (getDirection() == 0)
        {
            if (getWorld()->isBlockingObject(getX() - 1, getY()))
            {
                setDirection(180);
                return;
            }
            else if (getWorld()->isBlockingObject(getX() - 1 - SPRITE_WIDTH + 1, getY() - 1)) // need to make sure it doesn't go partly off the edge
                moveTo(getX() - 1, getY());
            else
            {
                setDirection(180);
                return;
            }
        }
        else
        {
            if (getWorld()->isBlockingObject(getX() + 1, getY()))
            {
                setDirection(0);
                return;
            }
            else if (getWorld()->isBlockingObject(getX() + 1 + SPRITE_WIDTH - 1, getY() - 1)) // need to make sure it doesn't go partly off the edge
                moveTo(getX() + 1, getY());
            else
            {
                setDirection(0);
                return;
            }
        }
    }
    
}

void Enemy::postDeath(){}

void Enemy::bonk()
{
    if (!getWorld()->isOverlapPeach(this))
        return;
    
    if (getWorld()->ifPeachStar()){
        getWorld()->playSound(SOUND_PLAYER_KICK);
        getWorld()->increaseScore(100);
        setDead();
        postDeath();
    }
}

bool Enemy::damage()
{
    getWorld()->increaseScore(100);
    setDead();
    postDeath();
    return true;
}

Goomba::Goomba(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Enemy(swp, imageID, startX, startY, startDirection, depth, size)
{}

Koopa::Koopa(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Enemy(swp, imageID, startX, startY, startDirection, depth, size)
{}

void Koopa::postDeath()
{
    getWorld()->addShell(getX(), getY(), getDirection());
}

Piranha::Piranha(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Enemy(swp, imageID, startX, startY, startDirection, depth, size)
{
    firing_delay = 0;
}

void Piranha::doSomething()
{
    if (!isAlive())
        return;
    
    increaseAnimationNumber();
    
    if (getWorld()->isOverlapPeach(this))
    {
        getWorld()->bonkPeach();
        return;
    }
    
    else
    {
        if (!getWorld()->ifPeachSameLevel(this))
            return;
        else
        {
            if (getWorld()->ifPeachLeft(this))
                setDirection(180); //i know this seems backwards but it seems like how it works?
            else
                setDirection(0);
            
            if (firing_delay > 0)
            {
                firing_delay--;
                return;
            }
            
            else
            {
                if (getWorld()->ifPeachInRange(this))
                {
                    getWorld()->addPiranhaFire(getX(), getY(), getDirection());
                    getWorld()->playSound(SOUND_PIRANHA_FIRE);
                    firing_delay = 40;
                }
            }
        }
    }
}

Projectile::Projectile(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Actor(swp, imageID, startX*SPRITE_WIDTH, startY*SPRITE_HEIGHT, startDirection, depth, size)
{}

void Projectile::bonk(){}

bool Projectile::blocks()
{
    return false;
}

void Projectile::doSomething()
{
    tryDamage();
    
    if (!getWorld()->isBlockingObject(getX(), getY() - 2))
    {
        moveTo(getX(), getY() - 2);
    }
    
    if (getDirection() == 0)
    {
        if (getWorld()->isBlockingObject(getX() - 2, getY()))
        {
            setDead();
            return;
        }
        moveTo(getX() - 2, getY());
    }
    else
    {
        if (getWorld()->isBlockingObject(getX() + 2, getY()))
        {
            setDead();
            return;
        }
        moveTo(getX() + 2, getY());
    }
}


void Projectile::tryDamage()
{
    if (getWorld()->damageActorAt(getX(), getY()))
    {
        setDead();
        return;
    }
}

PiranhaFire::PiranhaFire(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Projectile(swp, imageID, startX, startY, startDirection, depth, size)
{}

void PiranhaFire::tryDamage()
{
    if (getWorld()->isOverlapPeach(this))
    {
        getWorld()->damagePeach();
        setDead();
        return;
    }
}

PeachFire::PeachFire(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Projectile(swp, imageID, startX, startY, startDirection, depth, size)
{}

Shell::Shell(StudentWorld* swp, int imageID, int startX, int startY, int startDirection, int depth, double size): Projectile(swp, imageID, startX, startY, startDirection, depth, size)
{}

//all virtual destructors
Actor::~Actor(){}
Peach::~Peach(){}
Block::~Block(){}
mushroomBlock::~mushroomBlock(){}
flowerBlock::~flowerBlock(){}
starBlock::~starBlock(){}
Pipe::~Pipe(){}
Goodie::~Goodie(){}
Star::~Star(){}
Mushroom::~Mushroom(){}
Flower::~Flower(){}
Portal::~Portal(){}
Flag::~Flag(){}
Mario::~Mario(){}
Enemy::~Enemy(){}
Goomba::~Goomba(){}
Koopa::~Koopa(){}
Piranha::~Piranha(){}
Projectile::~Projectile(){}
PiranhaFire::~PiranhaFire(){}
PeachFire::~PeachFire(){}
Shell::~Shell(){}
