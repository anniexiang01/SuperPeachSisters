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
{
}

int StudentWorld::init() //construct representation of current level (populate with objects)
{
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
                        break;
                    case Level::koopa:
                        break;
                    case Level::piranha:
                        break;
                    case Level::block:
                        break;
                    case Level::star_goodie_block:
                        break;
                    case Level::mushroom_goodie_block:
                        break;
                    case Level::flower_goodie_block:
                        break;
                    case Level::pipe:
                        break;
                    case Level::flag:
                        break;
                    case Level::mario:
                        break;
                        
                }
            }
        }
     }
    
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    // This code is here merely to allow the game to build, run, and terminate after you hit enter.
    // Notice that the return value GWSTATUS_PLAYER_DIED will cause our framework to end the current level.
    
    vector<Actor*>:: iterator it;
    it = m_actor.begin();
    while (it != m_actor.end()) //all actors do something
    {
        if ((*it)->isAlive())
            (*it)->doSomething();
        it++;
    }
    
    m_peach->doSomething();
    
    if (!(m_peach->isAlive()))
        return GWSTATUS_PLAYER_DIED;
    
    if(m_peach->ifFlag())
    {
        playSound(SOUND_FINISHED_LEVEL);
        return GWSTATUS_FINISHED_LEVEL;
    }
    
    if(m_peach->ifMario())
    {
        playSound(SOUND_GAME_OVER);
        return GWSTATUS_PLAYER_WON;
    }
    
    //add actors when necessary (when Peach bonked a block to release goodie)
    
    
    //delete actors if m_status == false
    for (it = m_actor.begin(); it != m_actor.end();)
    {
        if ((*it)->isAlive())
            it++;
        else
            it = m_actor.erase(it);
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
    vector<Actor*>:: iterator it;
    while (it != m_actor.end()) //calls delete on all Actors, vector still have pointers but they point to nothing
    {
        delete *it;
        it++;
    }
    
    delete m_peach;
}

Peach* StudentWorld::getPeach()
{
    return m_peach;
}

bool StudentWorld::isBlockingObject(int x, int y)
{
    //iterate through vector of actors
    //if no matching getX and getY, return false
    //if match, if block or pipe, return true
    vector<Actor*>:: iterator it;
    while (it != m_actor.end())
    {
        if ((*it)->getX() == x && (*it)->getY() == y)
        {
            if ((*it)->isBlockOrPipe())
                return true;
            else
                return false;
        }
        it++;
    }
    return false;
}

Actor* StudentWorld::getActorAt(int x, int y)
{
    for (int i = 0; i < m_actor.size(); i++)
    {
        if (m_actor[i]->getX() == x && m_actor[i]->getY() == y)
            return m_actor[i];
    }
    return nullptr;
}
