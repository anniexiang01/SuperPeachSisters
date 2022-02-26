#include "StudentWorld.h"
#include "Actor.h"
#include "GameConstants.h"
#include "Level.h"
#include <string>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{}
StudentWorld::~StudentWorld()
{
    vector<Actor*>:: iterator it = m_actor.begin();
    while (it != m_actor.end()) //calls delete on all Actors, vector still have pointers but they point to nothing
    {
        delete *it;
        it++;
    }
    m_actor.clear();
    delete m_peach;
}

int StudentWorld::init() //construct representation of current level (populate with objects)
{
    flag = false;
    mario = false;
    shell = false;
    Level lev(assetPath());
    
    ostringstream oss; //use string streams to code level file through getLevel()
    oss << "level";
    oss.fill('0');
    oss << setw(2) << getLevel() << ".txt";
    string level_file = oss.str();
    
    Level::LoadResult result = lev.loadLevel(level_file);
    if (result == Level::load_fail_file_not_found)
     cerr << "Could not find data file" << endl;
    else if (result == Level::load_fail_bad_format)
     cerr << "file is improperly formatted" << endl;
    else if (result == Level::load_success)
    {
     cerr << "Successfully loaded level" << endl;
     Level::GridEntry ge;
        
    //loop through grid to initialize all actors
        for (int gy = GRID_HEIGHT-1; gy >= 0; gy--){
            for (int gx = 0; gx < GRID_WIDTH; gx++)
            {
                ge = lev.getContentsOf(gx, gy);
                switch (ge)
                {
                    case Level::empty:
                        break;
                    case Level::peach:
                        m_peach = new Peach(this, IID_PEACH, gx, gy, 0, 0, 1.0);
                        break;
                    case Level::goomba:
                        if (randInt(0, 1) == 0)
                            m_actor.push_back(new Goomba(this, IID_GOOMBA, gx, gy, 0, 1, 0));
                        else
                            m_actor.push_back(new Goomba(this, IID_GOOMBA, gx, gy, 180, 1, 0));
                        break;
                    case Level::koopa:
                        if (randInt(0, 1) == 0)
                            m_actor.push_back(new Koopa(this, IID_KOOPA, gx, gy, 0, 1, 0));
                        else
                            m_actor.push_back(new Koopa(this, IID_KOOPA, gx, gy, 180, 1, 0));
                        break;
                    case Level::piranha:
                        if (randInt(0, 1) == 0)
                            m_actor.push_back(new Piranha(this, IID_PIRANHA, gx, gy, 0, 1, 0));
                        else
                            m_actor.push_back(new Piranha(this, IID_PIRANHA, gx, gy, 180, 1, 0));
                        break;
                    case Level::block:
                        m_actor.push_back(new Block(this, IID_BLOCK, gx, gy, 0, 2, 1));
                        break;
                    case Level::star_goodie_block:
                        m_actor.push_back(new starBlock(this, IID_BLOCK, gx, gy, 0, 2, 1));
                        break;
                    case Level::mushroom_goodie_block:
                        m_actor.push_back(new mushroomBlock(this, IID_BLOCK, gx, gy, 0, 2, 1));
                        break;
                    case Level::flower_goodie_block:
                        m_actor.push_back(new flowerBlock(this, IID_BLOCK, gx, gy, 0, 2, 1));
                        break;
                    case Level::pipe:
                        m_actor.push_back(new Pipe(this, IID_PIPE, gx, gy, 0, 2, 1));
                        break;
                    case Level::flag:
                        m_actor.push_back(new Flag(this, IID_FLAG, gx, gy, 0, 1, 1));
                        break;
                    case Level::mario:
                        m_actor.push_back(new Mario(this, IID_MARIO, gx, gy, 0, 1, 1));
                        break;
                        
                }
            }
        }
     }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    
    
    vector<Actor*>:: iterator it;
    it = m_actor.begin();
    while (it != m_actor.end()) //all actors do something
    {
        if ((*it)->isAlive())
            (*it)->doSomething();
        it++;
    }
    
    //add shell if neccessary
    if (shell)
    {
        m_actor.push_back(new Shell(this, IID_SHELL, shellX/SPRITE_WIDTH, shellY/SPRITE_HEIGHT, shellDir, 1, 1));
        shell = false;
    }
    
    //add piranha fire if neccessary
    if (piranhaFire)
    {
        m_actor.push_back(new PiranhaFire(this, IID_PIRANHA_FIRE, pfireX/SPRITE_WIDTH, pfireY/SPRITE_HEIGHT, pfireDir, 1, 1));
        piranhaFire = false;
    }
    
    m_peach->doSomething();
    
    if (!(m_peach->isAlive())){
        decLives(); //not in spec but should be right?
        return GWSTATUS_PLAYER_DIED;
    }
    
    if(flag)
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if(mario)
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    
    //delete actors if m_status == false
    for (it = m_actor.begin(); it != m_actor.end();)
    {
        if ((*it)->isAlive())
            it++;
        else
        {
            delete *it;
            it = m_actor.erase(it);
        }
        
    }
    
    
    //update status text
    string status = "Lives: " + to_string(getLives()) + " Level: " + to_string(getLevel()) + " Points: " + to_string(getScore());
    if (m_peach->ifStar())
        status += " StarPower!";
    if (m_peach->ifShoot())
        status += " ShootPower!";
    if (m_peach->ifJump())
        status += " JumpPower!";
    setGameStatText(status);
    
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>:: iterator it = m_actor.begin();
    while (it != m_actor.end()) //calls delete on all Actors, vector still have pointers but they point to nothing
    {
        delete *it;
        it++;
    }
    m_actor.clear();
    delete m_peach;
}

bool StudentWorld::isBlockingObject(int x, int y)
{
    vector<Actor*>:: iterator it = m_actor.begin();
    while (it != m_actor.end())
    {
        if (isOverlap(*it, x, y))
        {
            if ((*it)->blocks())
                return true;
        }
        it++;
    }
    return false;
}

void StudentWorld::bonkActorAt(int x, int y)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (isOverlap(m_actor[i], x, y))
            m_actor[i]->bonk();
    }
}

void StudentWorld::damagePeach()
{
    m_peach->damage();
}

bool StudentWorld::damageActorAt(int x, int y)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (isOverlap(m_actor[i], x, y))
        {
            if (m_actor[i]->damage())
                return true;
        }
    }
    return false;
}

void StudentWorld::ifOverlapPeachBonk()
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (isOverlapPeach(m_actor[i]))
            m_actor[i]->bonk();
    }
}

void StudentWorld::bonkPeach()
{
    m_peach->bonk();
}

bool StudentWorld::isOverlap(Actor* a, Actor* b)
{
    int a_upper = a->getY() + SPRITE_HEIGHT - 1;
    int a_lower = a->getY();
    int a_left = a->getX();
    int a_right = a->getX() + SPRITE_WIDTH - 1;
    
    int b_upper = b->getY() + SPRITE_HEIGHT - 1;
    int b_lower = b->getY();
    int b_left = b->getX();
    int b_right = b->getX() + SPRITE_WIDTH - 1;
    
    if (a_upper < b_lower || a_lower > b_upper || a_left > b_right || a_right < b_left)
        return false;
    return true;
}

bool StudentWorld::isOverlap(Actor* a, int x, int y)
{
    int a_upper = a->getY()+SPRITE_HEIGHT-1;
    int a_lower = a->getY();
    int a_left = a->getX();
    int a_right = a->getX()+SPRITE_WIDTH-1;
    
    int b_upper = y+SPRITE_HEIGHT-1;
    int b_lower = y;
    int b_left = x;
    int b_right = x+SPRITE_WIDTH-1;
    
    if (a_upper < b_lower || a_lower > b_upper || a_left > b_right || a_right < b_left)
        return false;
    return true;
}

bool StudentWorld::isOverlapPeach(Actor* a)
{
    return isOverlap(a, m_peach);
}

bool StudentWorld::ifPeachStar()
{
    return m_peach->ifStar();
}

bool StudentWorld::ifPeachSameLevel(Actor* a)
{
    int lower = a->getY() - (1.5*SPRITE_HEIGHT);
    int upper = a->getY() + (1.5*SPRITE_HEIGHT);
    if (m_peach->getY() <= upper && m_peach->getY() >= lower)
        return true;
    else
        return false;
}

bool StudentWorld::ifPeachLeft(Actor* a)
{
    if (a->getX() > m_peach->getX())
        return true;
    else
        return false;
}

bool StudentWorld::ifPeachInRange(Actor* a)
{
    int distance;
    distance = a->getX() - m_peach->getX();
    
    if (distance < 0)
        distance *= -1;
    
    if (distance < (8*SPRITE_WIDTH))
        return true;
    else
        return false;
}

void StudentWorld::setStar()
{
    increaseScore(100);
    m_peach->setStar(150);
}

void StudentWorld::setShoot()
{
    increaseScore(50);
    m_peach->setShoot();
    m_peach->setHealth(2);
}

void StudentWorld::setJump()
{
    increaseScore(75);
    m_peach->setJump();
    m_peach->setHealth(2);
}

void StudentWorld::addMushroom(int x, int y)
{
    m_actor.push_back(new Mushroom(this, IID_MUSHROOM, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 180, 1, 1));
}

void StudentWorld::addStar(int x, int y)
{
    m_actor.push_back(new Star(this, IID_STAR, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 180, 1, 1));
}

void StudentWorld::addFlower(int x, int y)
{
    m_actor.push_back(new Flower(this, IID_FLOWER, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, 180, 1, 1));
}

void StudentWorld::nextLevel()
{
    increaseScore(1000);
    flag = true;
}

void StudentWorld::endGame()
{
    increaseScore(1000);
    mario = true;
}

void StudentWorld::addPiranhaFire(int x, int y, int dir)
{
    piranhaFire = true;
    pfireX = x;
    pfireY = y;
    if (dir == 0)
        pfireDir = 180;
    else
        pfireDir = 0;
}

void StudentWorld::addPeachFire(int x, int y, int dir)
{
    m_actor.push_back(new PeachFire(this, IID_PEACH_FIRE, x/SPRITE_WIDTH, y/SPRITE_HEIGHT, dir, 1, 1)); //can do this directly as peach is not in the vector of actors so adding an actor doesn't affect the iteration
}

void StudentWorld::addShell(int x, int y, int dir)
{
    shell = true;
    shellX = x;
    shellY = y;
    if (dir == 0)
        shellDir = 180;
    else
        shellDir = 0;
}
