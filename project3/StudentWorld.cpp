#include "StudentWorld.h"
#include "GameConstants.h"
#include "Actor.h"
#include <string>
#include <sstream>
using namespace std;

GameWorld* createStudentWorld(string assetPath)
{
	return new StudentWorld(assetPath);
}

// Students:  Add code to this file, StudentWorld.h, Actor.h, and Actor.cpp

StudentWorld::StudentWorld(string assetPath)
: GameWorld(assetPath)
{
    m_ghostracer = nullptr;
    soul_saved = 0;
    bonus_score = 5000;
    level_end = false;
}

StudentWorld::~StudentWorld()
{
    cleanUp();
}

int StudentWorld::init()
{
    GhostRacer* ghostracer = new GhostRacer(this);
    m_ghostracer = ghostracer;
    int N = VIEW_HEIGHT / SPRITE_HEIGHT;
    int M = VIEW_HEIGHT / (4 * SPRITE_HEIGHT);
    int LEFT_EDGE = ROAD_CENTER - ROAD_WIDTH / 2;
    int RIGHT_EDGE = ROAD_CENTER + ROAD_WIDTH / 2;
    for (int j = 0; j < N; j++)     //add yellow lines
    {
        BorderLine* left_yellow_borderline = new BorderLine(IID_YELLOW_BORDER_LINE, LEFT_EDGE, j * SPRITE_HEIGHT, this);
        BorderLine* right_yellow_borderline = new BorderLine(IID_YELLOW_BORDER_LINE, RIGHT_EDGE, j * SPRITE_HEIGHT, this);
        m_actor.push_back(left_yellow_borderline);
        m_actor.push_back(right_yellow_borderline);
    }
    for (int j = 0; j < M; j++)     //add white lines
    {
        BorderLine* left_white_borderline = new BorderLine(IID_WHITE_BORDER_LINE, LEFT_EDGE + ROAD_WIDTH / 3, j * 4 * SPRITE_HEIGHT, this);
        BorderLine* right_white_borderline = new BorderLine(IID_WHITE_BORDER_LINE, RIGHT_EDGE - ROAD_WIDTH / 3, j * 4 * SPRITE_HEIGHT, this);
        m_actor.push_back(left_white_borderline);
        m_actor.push_back(right_white_borderline);
    }
    return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move()
{
    vector<Actor*>::iterator it = m_actor.begin();
    m_ghostracer->doSomething();       //let ghost racer do something
    for (; it != m_actor.end(); it++)       //iterate through every actir
    {
        if ((*it)->is_alive())      //if it is still alive do something
            (*it)->doSomething();
        
        if (!m_ghostracer->is_alive())    //pause the game if ghost racer is dead
        {
            decLives();
            return GWSTATUS_PLAYER_DIED;
        }
        else if (get_soul_saved() >= getLevel() * 2 + 5)    //eneter next level if soul saved has meet the standard
        {
            increaseScore(get_bonus_score());
            return GWSTATUS_FINISHED_LEVEL;
        }
    }

    it = m_actor.begin();
    while (it != m_actor.end())     //delete all dead actors
    {
        if (!(*it)->is_alive())
        {
            delete* it;
            m_actor.erase(it);
            it = m_actor.begin();
        }
        else
            it++;
    }

    //add more actors
    add_borderline();
    add_human_ped();
    add_zombie_ped();
    add_zombie_cab();
    add_oil_slick();
    add_holy_water_goodie();
    add_soul_goodie();
 
    ostringstream oss;
    string text;
    oss << "Score: " << getScore() << "  " << "Lvl: " << getLevel() << "  " << "Souls2Save: " << getLevel() * 2 + 5 - get_soul_saved()
        << "  " << "Lives: " << getLives() << "  " << "Health: " << m_ghostracer->get_hit_point() << "  " << "Spray: " << m_ghostracer->get_holy_water_spray()
        << "  " << "Bonus: " << get_bonus_score();
    text = oss.str();
    setGameStatText(text);    //set the text in the upper middle
    if (get_bonus_score() > 0)
        set_bonus_score(get_bonus_score() - 1);
    return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp()
{
    vector<Actor*>::iterator it = m_actor.begin();    //free all the memory
    while (it != m_actor.end())
    {
        delete* it;
        m_actor.erase(it);
        it = m_actor.begin();
    }
    delete m_ghostracer;
}

GhostRacer* StudentWorld::getghostracer() const
{
    return m_ghostracer;
}

int StudentWorld::get_soul_saved() const
{
    return soul_saved;
}

void StudentWorld::save_a_soul()
{
    soul_saved++;
}

int StudentWorld::get_bonus_score() const
{
    return bonus_score;
}

void StudentWorld::set_bonus_score(int score)
{
    bonus_score = score;
}

vector<Actor*> StudentWorld::get_m_actor()
{
    return m_actor;
}

void StudentWorld::add_healing_goodie(double x_cor, double y_cor)
{
    HealingGoodie* new_healinggoodie = new HealingGoodie(x_cor, y_cor, this);
    m_actor.push_back(new_healinggoodie);
}

void StudentWorld::add_oil_slick(double x_cor, double y_cor)
{
    OilSlick* new_oilslick = new OilSlick(x_cor, y_cor, this);
    m_actor.push_back(new_oilslick);
}

void StudentWorld::add_holy_water_projectile(double x_cor, double y_cor, int dir)
{
    HolyWaterProjectile* new_holywaterprojectile = new HolyWaterProjectile(x_cor, y_cor, dir, this);
    m_actor.push_back(new_holywaterprojectile);
}

void StudentWorld::add_borderline()
{
    int new_border_y = VIEW_HEIGHT - SPRITE_HEIGHT;
    vector<Actor*>::iterator it = m_actor.end();
    it--;
    while (it != m_actor.begin())    //find the latest added white borderline
    {
        if ((*it)->get_hit_point() == -1 && ((*it)->getX() == ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3 || (*it)->getX() == ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3))
            break;
        it--;
    }
    int delta_y = new_border_y - (*it)->getY();
    if (delta_y >= SPRITE_HEIGHT)
    {
        BorderLine* left_yellow_borderline = new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2, new_border_y, this);
        BorderLine* right_yellow_borderline = new BorderLine(IID_YELLOW_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2, new_border_y, this);
        m_actor.push_back(left_yellow_borderline);
        m_actor.push_back(right_yellow_borderline);
    }
    if (delta_y >= 4 * SPRITE_HEIGHT)
    {
        BorderLine* left_white_borderline = new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3, new_border_y, this);
        BorderLine* right_white_borderline = new BorderLine(IID_WHITE_BORDER_LINE, ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3, new_border_y, this);
        m_actor.push_back(left_white_borderline);
        m_actor.push_back(right_white_borderline);
    }
}

void StudentWorld::add_human_ped()
{
    int ChanceHumanPed = max(200 - getLevel() * 10, 30);
    int rand_int = randInt(0, ChanceHumanPed - 1);
    if (rand_int == 0)
    {
        double x_cor = randInt(0, VIEW_WIDTH);
        double y_cor = VIEW_HEIGHT;
        HumanPedestrian* newhuman = new HumanPedestrian(x_cor, y_cor, this);
        m_actor.push_back(newhuman);
    }
}

void StudentWorld::add_zombie_ped()
{
    int ChanceZombiePed = max(100 - getLevel() * 10, 20);
    int rand_int = randInt(0, ChanceZombiePed - 1);
    if (rand_int == 0)
    {
        double x_cor = randInt(0, VIEW_WIDTH);
        double y_cor = VIEW_HEIGHT;
        ZombiePedestrian* newzombie = new ZombiePedestrian(x_cor, y_cor, this);
        m_actor.push_back(newzombie);
    }
}

void StudentWorld::add_zombie_cab()
{
    int ChanceVehicle = max(100 - getLevel() * 10, 20);
    int rand_int = randInt(0, ChanceVehicle - 1);
    if (rand_int == 0)
    {
        vector<Actor*>::iterator it = m_actor.begin();
        int cur_lane = randInt(1, 3);       //randomly pick a line to first add the zombie cab
        for (int i = 1; i <= 3; i++)
        {
            int x_cor = get_lane_x_coordinate(cur_lane);
            while (it != m_actor.end())
            {
                if ((*it)->get_cawa() && (*it)->in_which_line() == cur_lane && (*it)->getY() <= VIEW_HEIGHT / 3)    //there shouln't be cawa actors in the same line close enough
                    break;
                else if (m_ghostracer->in_which_line() == cur_lane)
                    break;
                it++;
            }
            if (it == m_actor.end())
            {
                ZombieCab* new_cab = new ZombieCab(x_cor, SPRITE_HEIGHT / 2, m_ghostracer->get_v_speed() + randInt(2, 4), this);
                m_actor.push_back(new_cab);
                break;
            }

            it = m_actor.begin();
            while (it != m_actor.end())
            {
                if ((*it)->get_cawa() && (*it)->in_which_line() == cur_lane && (*it)->getY() >= VIEW_HEIGHT * 2 / 3)
                    break;
                it++;
            }
            if (it == m_actor.end())
            {
                ZombieCab* new_cab = new ZombieCab(x_cor, VIEW_HEIGHT - SPRITE_HEIGHT / 2, m_ghostracer->get_v_speed() - randInt(2, 4), this);
                m_actor.push_back(new_cab);
                break;
            }

            if (cur_lane == 3)    //go over the rest of three lines
                cur_lane = 1;
            else
                cur_lane++;
        }
    }
}

void StudentWorld::add_oil_slick()
{
    int ChanceOilSlick = max(150 - getLevel() * 10, 40);
    int rint = randInt(0, ChanceOilSlick - 1);
    if (rint == 0)
    {
        int x_cor = randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2);
        OilSlick* new_oilslick = new OilSlick(x_cor, VIEW_HEIGHT, this);
        m_actor.push_back(new_oilslick);
    }
}

void StudentWorld::add_holy_water_goodie()
{
    int ChanceOfHolyWater = 100 + 10 * getLevel();
    int rint = randInt(0, ChanceOfHolyWater - 1);
    if (rint == 0)
    {
        int x_cor = randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2);
        HolyWaterGoodie* new_holywater = new HolyWaterGoodie(x_cor, VIEW_HEIGHT, this);
        m_actor.push_back(new_holywater);
    }
}

void StudentWorld::add_soul_goodie()
{
    int ChanceOfLostSoul = 100;
    int rint = randInt(0, ChanceOfLostSoul - 1);
    if (rint == 0)
    {
        int x_cor = randInt(ROAD_CENTER - ROAD_WIDTH / 2, ROAD_CENTER + ROAD_WIDTH / 2);
        SoulGoodie* new_soul = new SoulGoodie(x_cor, VIEW_HEIGHT, this);
        m_actor.push_back(new_soul);
    }
}

double StudentWorld::get_lane_x_coordinate(int n) const
{
    if (n == 1)
        return ROAD_CENTER - ROAD_WIDTH / 3;
    else if (n == 2)
        return ROAD_CENTER;
    else if (n == 3)
        return ROAD_CENTER + ROAD_WIDTH / 3;
    else
        return 0;
}