#include "Actor.h"
#include "StudentWorld.h"
#include <vector>

//implementation of Actor base class derived from GraphObject class
Actor::Actor(StudentWorld* studentworld, bool al, int v_speed, int h_speed, int h_point, bool cawa,
	int ID, double start_x, double start_y, int dir, double size, unsigned int depth)
	: GraphObject(ID, start_x, start_y, dir, size, depth)
{
	alive = al;
	vertical_speed = v_speed;
	horizontal_speed = h_speed;
	hit_point = h_point;
	collision_avoidance_worthy_actor = cawa;
	m_studentworld = studentworld;
}

bool Actor::is_alive() const
{
	return alive;
}

void Actor::set_alive(bool al)
{
	alive = al;
}

int Actor::get_v_speed() const
{
	return vertical_speed;
}

void Actor::set_v_speed(int v_speed)
{
	vertical_speed = v_speed;
}

int Actor::get_h_speed() const
{
	return horizontal_speed;
}

void Actor::set_h_speed(int h_speed)
{
	horizontal_speed = h_speed;
}

int Actor::get_hit_point() const
{
	return hit_point;
}

void Actor::set_hit_point(int hitpoint)
{
	hit_point = hitpoint;
}

bool Actor::get_cawa() const
{
	return collision_avoidance_worthy_actor;
}

StudentWorld* Actor::get_studentworld() const
{
	return m_studentworld;
}

bool Actor::is_overlap(Actor* ac) const		//determine wehther is overlap with ac 
{
	double delta_x = fabs(getX() - ac->getX());
	double delta_y = fabs(getY() - ac->getY());
	double radius_sum = getRadius() + ac->getRadius();
	if (delta_x < radius_sum * 0.25 && delta_y < radius_sum * 0.6)
		return true;
	else
		return false;
}

int Actor::in_which_line() const	//determine in hwich line
{
	if (getX() >= ROAD_CENTER - ROAD_WIDTH / 2 && getX() < ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3)
		return 1;
	else if (getX() < ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3 && getX() >= ROAD_CENTER - ROAD_WIDTH / 2 + ROAD_WIDTH / 3)
		return 2;
	else if (getX() >= ROAD_CENTER + ROAD_WIDTH / 2 - ROAD_WIDTH / 3 && getX() < ROAD_CENTER + ROAD_WIDTH / 2)
		return 3;
	else
		return 0;
}

void Actor::m_move()	//all actors (except ghost racer) use the same moving algorithm
{
	int vert_speed = get_v_speed() - get_studentworld()->getghostracer()->get_v_speed();
	int horiz_speed = get_h_speed();
	int new_y = getY() + vert_speed;
	int new_x = getX() + horiz_speed;
	moveTo(new_x, new_y);
	if (getX() < 0 || getY() < 0 || getX() > VIEW_WIDTH || getY() > VIEW_HEIGHT)
		set_alive(false);
}

//implementation of GhostRacer derived from Actor
GhostRacer::GhostRacer(StudentWorld* studentworld, int hws, bool al, int v_speed, int h_speed, int h_point, bool cawa,
	int ID, double start_x, double start_y, int dir, double size, unsigned int depth)
	: Actor(studentworld, al, v_speed, h_speed, h_point, cawa, ID, start_x, start_y, dir, size, depth)
{
	holy_water_spray = hws;
}

void GhostRacer::doSomething()
{
	if (get_hit_point() <= 0)
		return;

	int ch;
	if (getX() <= ROAD_CENTER - ROAD_WIDTH / 2)		//check if it is over the left border
	{
		if (getDirection() > 90)
		{
			set_hit_point(get_hit_point() - 10);
			if (check_hit_point())
				return;
		}
		setDirection(82);
		get_studentworld()->playSound(SOUND_VEHICLE_CRASH);
	}
	else if (getX() >= ROAD_CENTER + ROAD_WIDTH / 2)	//check if it is over the right border
	{
		if (getDirection() < 90)
		{
			set_hit_point(get_hit_point() - 10);
			if (check_hit_point())
				return;
		}
		setDirection(98);
		get_studentworld()->playSound(SOUND_VEHICLE_CRASH);
	}
	else if (get_studentworld()->getKey(ch))	//check which key did the user press
	{
		switch (ch)
		{
		case KEY_PRESS_SPACE:		//shoot holy water spray
			if (get_holy_water_spray() >= 1)
			{
				double x_cor = getX() + SPRITE_HEIGHT * cos(getDirection() / 180.0 * 3.14159);
				double y_cor = getY() + SPRITE_HEIGHT * sin(getDirection() / 180.0 * 3.14159);
				int direc = getDirection();
				get_studentworld()->add_holy_water_projectile(x_cor, y_cor, direc);
				get_studentworld()->playSound(SOUND_PLAYER_SPRAY);
				set_holy_water_spray(get_holy_water_spray() - 1);
			}
			break;
		case KEY_PRESS_LEFT:		//turn left
			if (getDirection() < 114)
			{
				if (getDirection() < 90 && getDirection() + 8 > 90)
					setDirection(90);
				else
					setDirection(getDirection() + 8);
			}
			break;
		case KEY_PRESS_RIGHT:		//turn right
			if (getDirection() > 66)
			{
				if (getDirection() > 90 && getDirection() - 8 < 90)
					setDirection(90);
				else
					setDirection(getDirection() - 8);
			}
			break;
		case KEY_PRESS_UP:		    //spedd up
			if (get_v_speed() < 5)
				set_v_speed(get_v_speed() + 1);
			break;
		case KEY_PRESS_DOWN:		//speed down
			if (get_v_speed() > -1)
				set_v_speed(get_v_speed() - 1);
			break;
		}
	}
	m_move();
}

void GhostRacer::m_move()		//ghost racer has its own algorithm
{
	double max_shift_per_tick = 4.0;
	int direction = getDirection();
	double delta_x;
	delta_x = cos((direction / 180.0) * 3.1415926) * max_shift_per_tick;
	double cur_x = getX();
	double cur_y = getY();
	moveTo(cur_x + delta_x, cur_y);
}

void GhostRacer::set_holy_water_spray(int hws)
{
	holy_water_spray = hws;
}

int GhostRacer::get_holy_water_spray() const
{
	return holy_water_spray;
}

bool GhostRacer::check_hit_point()		//set to not alive if hit point is below zero
{
	if (get_hit_point() <= 0)
	{
		set_alive(false);
		get_studentworld()->playSound(SOUND_PLAYER_DIE);
		return true;
	}
	else
		return false;
}

//implementation of Borderline derived from Actor
BorderLine::BorderLine(int ID, double start_x, double start_y, StudentWorld* studentworld, bool al, int v_speed, int h_speed, int h_point,
	bool cawa, int dir, double size, unsigned int depth)
	: Actor(studentworld, al, v_speed, h_speed, h_point, cawa, ID, start_x, start_y, dir, size, depth)
{ }

void BorderLine::doSomething()
{
	m_move();
}

//implementation of MovingWalkingActor (for zombie and pedestrian)
MovingWalkingActor::MovingWalkingActor(double start_x, double start_y, double size, int ID, StudentWorld* studentworld, double mpd, bool al, int v_speed, int h_speed,
	int h_point, bool cawa, int dir, unsigned int depth)
	: Actor(studentworld, al, v_speed, h_speed, h_point, cawa, ID, start_x, start_y, dir, size, depth)
{
	movement_plan_distance = mpd;
}

void MovingWalkingActor::set_mpd(int n)
{
	movement_plan_distance = randInt(4, 32);
	if (n == 3)		//human & zombie pedestrian
	{
		int h_speed = randInt(-3, 3);
		while (h_speed == 0)
			h_speed = randInt(-3, 3);
		set_h_speed(h_speed);
		int direction = (get_h_speed() < 0) ? 180 : 0;
		setDirection(direction);
	}
	else		//zombie cab
		set_v_speed(get_v_speed() + randInt(-2, 2));
}

double MovingWalkingActor::get_mpd() const
{
	return movement_plan_distance;
}

bool MovingWalkingActor::dec_mpd()
{
	if (movement_plan_distance > 0)     //decrease it by 1 if m_p_d is greater than 0
	{
		movement_plan_distance--;
		return true;
	}
	else
		return false;
}

void MovingWalkingActor::doSomething()		//all movingwalkingactors follow the same sequence of so something
{
	if (!is_alive())
		return;

	if (is_overlap(get_studentworld()->getghostracer()))	//do something if it is overlap with ghostracer
		if (OverlapDoSomething())
			return;

	if (getSize() == 3.0)     //zoombie pedestrian special case
		CheckZpClose();

	Actor::m_move();
	if (!is_alive())
		return;

	if (getSize() == 4.0)    //zpmbie cab special case
		if (CheckZcClose())
			return;

	if (MovingWalkingActor::dec_mpd())
		return;
	else if (getSize() == 4.0)		//use size to distinguish between pedestrian and cab
		MovingWalkingActor::set_mpd(2);
	else
		MovingWalkingActor::set_mpd(3);
}

//implementation of HumanPedestrian derived from MovingWalkingActor
HumanPedestrian::HumanPedestrian(double start_x, double start_y, StudentWorld* studentworld, double size, int ID)
	: MovingWalkingActor(start_x, start_y, size, ID, studentworld)
{}

bool HumanPedestrian::OverlapDoSomething()		//ghostracer is dead if it is overlap with the human ped
{
	get_studentworld()->getghostracer()->set_alive(false);
	return true;
}

//implementation of ZombiePedestrian derived from MovingWalkingActor
ZombiePedestrian::ZombiePedestrian(double start_x, double start_y, StudentWorld* studentworld, int t, double size, int ID)
	: MovingWalkingActor(start_x, start_y, size, ID, studentworld)
{
	ticks = t;
}

void ZombiePedestrian::set_ticks(int tick)
{
	ticks = tick;
}

int ZombiePedestrian::get_ticks() const
{
	return ticks;
}

bool ZombiePedestrian::OverlapDoSomething()
{
	get_studentworld()->getghostracer()->set_hit_point(get_studentworld()->getghostracer()->get_hit_point() - 5);     //decrease ghostracer hit point by 5
	if (get_studentworld()->getghostracer()->check_hit_point())
		return true;
	set_hit_point(get_hit_point() - 2);		//decrease zombie ped hit point by 2
	if (get_hit_point() <= 0)
	{
		set_alive(false);
		get_studentworld()->playSound(SOUND_PED_DIE);
		get_studentworld()->increaseScore(150);
	}
	else
		get_studentworld()->playSound(SOUND_PED_HURT);
	return true;
}

void ZombiePedestrian::CheckZpClose()
{
	double delta_x = getX() - get_studentworld()->getghostracer()->getX();		//zombie will turn to attack ghostrace if it is close enough
	if (delta_x <= 30.0 && delta_x >= -30.0 && getY() > get_studentworld()->getghostracer()->getY())
	{
		setDirection(270);
		if (delta_x <= 30.0 && delta_x > 0.0)
			set_h_speed(-1);
		else if (delta_x >= -30.0 && delta_x < 0.0)
			set_h_speed(1);
		else
			set_h_speed(0);
		set_ticks(get_ticks() - 1);
		if (ticks <= 0)
		{
			get_studentworld()->playSound(SOUND_ZOMBIE_ATTACK);
			set_ticks(20);
		}
	}
}

//implementation of ZombieCab derived from WalkingMovingActor
ZombieCab::ZombieCab(double start_x, double start_y, int v_speed, StudentWorld* studentworld, int ID, bool d_g, double size, double mpd, bool al, int h_speed,
	int h_point, bool cawa, int dir, unsigned int depth)
	: MovingWalkingActor(start_x, start_y, size, ID, studentworld, mpd, al, v_speed, h_speed, h_point, cawa, dir, depth)
{
	damaged_ghostracer = d_g;
}

void ZombieCab::set_damaged_ghostracer(bool d_g)
{
	damaged_ghostracer = d_g;
}

bool ZombieCab::get_damaged_ghostracer() const
{
	return damaged_ghostracer;
}

bool ZombieCab::OverlapDoSomething()
{
	if (!get_damaged_ghostracer())		//zombie cab will slide and crash when it is overlap with the ghost racer
	{
		get_studentworld()->playSound(SOUND_VEHICLE_CRASH);
		get_studentworld()->getghostracer()->set_hit_point(get_studentworld()->getghostracer()->get_hit_point() - 20);
		if (get_studentworld()->getghostracer()->check_hit_point())
			return true;
		if (getX() <= get_studentworld()->getghostracer()->getX())
		{
			set_h_speed(-5);
			setDirection(120 + randInt(0, 19));
		}
		if (getX() > get_studentworld()->getghostracer()->getX())
		{
			set_h_speed(5);
			setDirection(60 - randInt(0, 19));
		}
		set_damaged_ghostracer(true);
	}
	return false;
}

bool ZombieCab::CheckZcClose()
{
	std::vector<Actor*> temp = get_studentworld()->get_m_actor();		//zombie cab has to check collision avoidance warning actors during its movement to adjust its speed
	std::vector<Actor*>::iterator it = temp.begin();
	if (get_v_speed() > get_studentworld()->getghostracer()->get_v_speed())
	{
		while (it != temp.end())
		{
			if ((*it)->get_cawa() && (*it)->getY() > getY() && (*it)->in_which_line() == in_which_line() && (*it)->getY() - getY() < 96)	//cawa + in front of it + same line + close enough
			{
				set_v_speed(get_v_speed() - 0.5);
				return true;
			}
			it++;
		}
	}
	else if (get_v_speed() <= get_studentworld()->getghostracer()->get_v_speed())
	{
		while (it != temp.end())
		{
			if ((*it)->get_cawa() && (*it)->getY() < getY() && (*it)->in_which_line() == in_which_line() && getY() - (*it)->getY() < 96)	//cawa + behind it + same line + close enough
			{
				set_v_speed(get_v_speed() + 0.5);
				return true;
			}
			it++;
		}
	}
	return false;
}

//implementation of Goodie derieved from Actor
Goodie::Goodie(double start_x, double start_y, double size, int dir, int ID, int h_point, StudentWorld* studentworld, bool al,
	unsigned int depth, int h_speed, int v_speed, bool cawa)
	: Actor(studentworld, al, v_speed, h_speed, h_point, cawa, ID, start_x, start_y, dir, size, depth)
{}

void Goodie::doSomething()		//all goodie follow the same sequence of doing something
{
	Actor::m_move();
	if (!is_alive())
		return;
	if (is_overlap(get_studentworld()->getghostracer()))
		GoodieDoSomething();
	if (get_hit_point() == -4)		//especially for soul goodie
		setDirection(getDirection() - 10);
}

//implementaion of OilSlick derived from Goodie
OilSlick::OilSlick(double start_x, double start_y, StudentWorld* studentworld, double size, int dir, int ID, int h_point)
	: Goodie(start_x, start_y, size, dir, ID, h_point, studentworld)
{}

void OilSlick::GoodieDoSomething()		//oil slick has to slide the ghost racer when overlap
{
	get_studentworld()->playSound(SOUND_OIL_SLICK);
	int delta_dir = randInt(5, 20);		 //with random degree
	int left_or_right = randInt(1, 2);   //randomly to left or right
	int new_dir = 0;
	if (left_or_right == 1)		
	{
		new_dir = get_studentworld()->getghostracer()->getDirection() + delta_dir;
		if (new_dir > 120)
			new_dir = 120;
	}
	else
	{
		new_dir = get_studentworld()->getghostracer()->getDirection() - delta_dir;
		if (new_dir < 60)
			new_dir = 60;
	}
	get_studentworld()->getghostracer()->setDirection(new_dir);
}

//implementation of HealingGoodie derived from Goodie
HealingGoodie::HealingGoodie(double start_x, double start_y, StudentWorld* studentworld, double size, int dir, int ID, int h_point)
	: Goodie(start_x, start_y, size, dir, ID, h_point, studentworld)
{}

void HealingGoodie::GoodieDoSomething()
{
	get_studentworld()->getghostracer()->set_hit_point(get_studentworld()->getghostracer()->get_hit_point() + 10);		//heal by 10 hit point
	if (get_studentworld()->getghostracer()->get_hit_point() > 100)		//should not exceed 100 hit point
		get_studentworld()->getghostracer()->set_hit_point(100);
	set_alive(false);
	get_studentworld()->playSound(SOUND_GOT_GOODIE);
	get_studentworld()->increaseScore(250);
}

//implementation of HolyWaterGoodie derived from Goodie
HolyWaterGoodie::HolyWaterGoodie(double start_x, double start_y, StudentWorld* studentworld, double size, int dir, int ID, int h_point)
	: Goodie(start_x, start_y, size, dir, ID, h_point, studentworld)
{}

void HolyWaterGoodie::GoodieDoSomething()
{
	get_studentworld()->getghostracer()->set_holy_water_spray(get_studentworld()->getghostracer()->get_holy_water_spray() + 10);		//refill holy water by 10
	set_alive(false);
	get_studentworld()->playSound(SOUND_GOT_GOODIE);
	get_studentworld()->increaseScore(50);
}

//implementation of SoulGoodie derived form Goodie
SoulGoodie::SoulGoodie(double start_x, double start_y, StudentWorld* studentworld, double size, int dir, int ID, int h_point)
	: Goodie(start_x, start_y, size, dir, ID, h_point, studentworld)
{}

void SoulGoodie::GoodieDoSomething()
{
	get_studentworld()->save_a_soul();		//increase soul saved by 1
	set_alive(false);
	get_studentworld()->playSound(SOUND_GOT_SOUL);
	get_studentworld()->increaseScore(100);
}

//implementation of HolyWaterProjectile derived from Actor
HolyWaterProjectile::HolyWaterProjectile(double start_x, double start_y, int dir, StudentWorld* studentworld, int v_speed, int h_speed, int dt, int mtd, double size,
	int ID, bool al, unsigned int depth, int h_point, bool cawa)
	: Actor(studentworld, al, v_speed, h_speed, h_point, cawa, ID, start_x, start_y, dir, size, depth)
{
	max_travel_distance = mtd;
	distance_travel = dt;
}

void HolyWaterProjectile::doSomething()
{
	if (!is_alive())
		return;

	std::vector<Actor*> m_actor = get_studentworld()->get_m_actor();
	std::vector<Actor*>::iterator it = m_actor.begin();
	for (; it != m_actor.end(); it++)		//iterate through all the actors in m_actor, distinguihsh them by hit point and size
	{
		if (is_overlap(*it))
		{
			if ((*it)->get_hit_point() == 2 && (*it)->getSize() == 2.0) //human ped
			{
				(*it)->set_h_speed((*it)->get_h_speed() * -1);		//change speed and direction, but not hurt
				(*it)->setDirection((*it)->getDirection() + 180);
				get_studentworld()->playSound(SOUND_PED_HURT);
				set_alive(false);
				return;
			}
			if ((*it)->getSize() == 3.0 && ((*it)->get_hit_point() == 2 || (*it)->get_hit_point() == 1))   //zombie ped
			{
				(*it)->set_hit_point((*it)->get_hit_point() - 1);		//damage it by one
				if ((*it)->get_hit_point() <= 0)
				{
					(*it)->set_alive(false);
					get_studentworld()->playSound(SOUND_PED_DIE);
					if (!(*it)->is_overlap(get_studentworld()->getghostracer()) && randInt(1, 5) == 1)    //one of fifth possiobility to add healing goodie
						get_studentworld()->add_healing_goodie((*it)->getX(), (*it)->getY());
					get_studentworld()->increaseScore(150);
				}
				else
					get_studentworld()->playSound(SOUND_PED_HURT);
				set_alive(false);
				return;
			}
			if ((*it)->getSize() == 4.0 && (*it)->get_hit_point() <= 3 && (*it)->get_hit_point() > 0)     //zombie cab
			{
				(*it)->set_hit_point((*it)->get_hit_point() - 1);		//damage it by one
				if ((*it)->get_hit_point() <= 0)
				{
					(*it)->set_alive(false);
					get_studentworld()->playSound(SOUND_VEHICLE_DIE);
					if (!(*it)->is_overlap(get_studentworld()->getghostracer()) && randInt(1, 5) == 1)     //one of fifth possibility to add oil slick
						get_studentworld()->add_oil_slick((*it)->getX(), (*it)->getY());
					get_studentworld()->increaseScore(200);
				}
				else
					get_studentworld()->playSound(SOUND_VEHICLE_HURT);
				set_alive(false);
				return;
			}
			if ((*it)->get_hit_point() == -2)    //healing goodie & holy water goodie 
			{
				(*it)->set_alive(false);
				set_alive(false);
				return;
			}
		}
	}

	moveForward(SPRITE_HEIGHT);		//holy water projectile travels by same amount every time
	distance_travel += SPRITE_HEIGHT;
	if (getX() > VIEW_WIDTH || getX() < 0.0 || getY() < 0.0 || getY() > VIEW_HEIGHT)
	{
		set_alive(false);
		return;
	}
	if (distance_travel >= max_travel_distance)
	{
		set_alive(false);
		return;
	}
}