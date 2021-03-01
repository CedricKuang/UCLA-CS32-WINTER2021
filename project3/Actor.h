#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "GameConstants.h"

class StudentWorld;

class Actor : public GraphObject
{
public:
	Actor(StudentWorld* studentworld, bool al, int v_speed, int h_speed, int h_point, bool cawa,
		int ID, double start_x, double start_y, int dir, double size, unsigned int depth);
	virtual ~Actor() {}
	virtual void doSomething() = 0;
	virtual void m_move();
	bool is_alive() const;
	void set_alive(bool set);
	int get_v_speed() const;
	void set_v_speed(int v_speed);
	int get_h_speed() const;
	void set_h_speed(int h_speed);
	int get_hit_point() const;
	void set_hit_point(int hitpoint);
	bool get_cawa() const;
	bool is_overlap(Actor* ac) const;
	int in_which_line() const;
	StudentWorld* get_studentworld() const;
private:
	bool alive;
	int vertical_speed;
	int horizontal_speed;
	int hit_point;
	bool collision_avoidance_worthy_actor;
	StudentWorld* m_studentworld;		//pointer to current student world
};

class GhostRacer : public Actor
{
public:
	GhostRacer(StudentWorld* studentworld, int hws = 10, bool al = true, int v_speed = 0, int h_speed = 0, int h_point = 100, bool cawa = true,
		int ID = IID_GHOST_RACER, double start_x = 128, double start_y = 32, int dir = 90, double size = 4.0, unsigned int depth = 0);
	virtual ~GhostRacer() {};
	virtual void doSomething();
	virtual void m_move();
	void set_holy_water_spray(int hws);
	int get_holy_water_spray() const;
	bool check_hit_point();
private:
	int holy_water_spray;
};

class BorderLine : public Actor
{
public:
	BorderLine(int ID, double start_x, double start_y, StudentWorld* studentworld, bool al = true, int v_speed = -4, int h_speed = 0, int h_point = -1,
		bool cawa = false, int dir = 0, double size = 2.0, unsigned int depth = 2);
	virtual ~BorderLine() {}
	virtual void doSomething();
};

class MovingWalkingActor : public Actor
{
public:
	MovingWalkingActor(double start_x, double start_y, double size, int ID, StudentWorld* studentworld, double mpd = 0.0, bool al = true, int v_speed = -4, int h_speed = 0,
		int h_point = 2, bool cawa = true, int dir = 0, unsigned int depth = 0);
	virtual ~MovingWalkingActor() {}
    void set_mpd(int n);
	double get_mpd() const;
	bool dec_mpd();
	virtual void doSomething();
	virtual bool OverlapDoSomething() = 0;
	virtual void CheckZpClose() {};			//only for Zombie pedestrian
	virtual bool CheckZcClose() { return true; };		//only for Zombie Cab
private:
	double movement_plan_distance;
};

class HumanPedestrian : public MovingWalkingActor
{
public:
	HumanPedestrian(double start_x, double start_y, StudentWorld* studentworld, double size = 2.0, int ID = IID_HUMAN_PED);
	virtual ~HumanPedestrian() {}
	virtual bool OverlapDoSomething();
};

class ZombiePedestrian : public MovingWalkingActor
{
public:
	ZombiePedestrian(double start_x, double start_y, StudentWorld* studentworld, int t = 0, double size = 3.0, int ID = IID_ZOMBIE_PED);
	virtual ~ZombiePedestrian() {}
	void set_ticks(int tick);
	int get_ticks() const;
	virtual bool OverlapDoSomething();
	virtual void CheckZpClose();
private:
	int ticks;
};

class ZombieCab : public MovingWalkingActor
{
public:
	ZombieCab(double start_x, double start_y, int v_speed, StudentWorld* studentworld, int ID = IID_ZOMBIE_CAB, bool d_g = false, double size = 4.0, double mpd = 0.0, bool al = true,
	   int h_speed = 0, int h_point = 3, bool cawa = true, int dir = 90, unsigned int depth = 0);
	virtual ~ZombieCab() {}
	void set_damaged_ghostracer(bool d_g);
	bool get_damaged_ghostracer() const;
	virtual bool OverlapDoSomething();
	virtual bool CheckZcClose();
private:
	bool damaged_ghostracer;
};

class Goodie : public Actor
{
public:
	Goodie(double start_x, double start_y, double size, int dir, int ID, int h_point, StudentWorld* studentworld, bool al = true,
		unsigned int depth = 2, int h_speed = 0, int v_speed = -4, bool cawa = false);
	virtual ~Goodie() {}
	virtual void doSomething();
	virtual void GoodieDoSomething() = 0;
};

class OilSlick : public Goodie
{
public:
	OilSlick(double start_x, double start_y, StudentWorld* studentworld, double size = randInt(2, 5), int dir = 0, int ID = IID_OIL_SLICK, int h_point = -3);
	virtual ~OilSlick() {}
	virtual void GoodieDoSomething();
};

class HealingGoodie : public Goodie   //add when holy water destroy zombie pedestrian
{
public:
	HealingGoodie(double start_x, double start_y, StudentWorld* studentworld, double size = 1.0, int dir = 0, int ID = IID_HEAL_GOODIE, int h_point = -2);
	virtual ~HealingGoodie() {}
	virtual void GoodieDoSomething();
};

class HolyWaterGoodie : public Goodie
{
public:
	HolyWaterGoodie(double start_x, double start_y, StudentWorld* studentworld, double size = 2.0, int dir = 90, int ID = IID_HOLY_WATER_GOODIE, int h_point = -2);
	virtual ~HolyWaterGoodie() {}
	virtual void GoodieDoSomething();
};

class SoulGoodie : public Goodie
{
public:
	SoulGoodie(double start_x, double start_y, StudentWorld* studentworld, double size = 4.0, int dir = 0, int ID = IID_SOUL_GOODIE, int h_point = -4);
	virtual ~SoulGoodie() {}
	virtual void GoodieDoSomething();
};

class HolyWaterProjectile : public Actor
{
public:
	HolyWaterProjectile(double start_x, double start_y, int dir, StudentWorld* studentworld, int v_speed = 0, int h_speed = 0, int dt = 0, int mtd = 160, double size = 1.0,
		int ID = IID_HOLY_WATER_PROJECTILE, bool al = true, unsigned int depth = 1, int h_point = -5, bool cawa = false);
	virtual ~HolyWaterProjectile() {}
	virtual void doSomething();
private:
	int max_travel_distance;
	int distance_travel;
};

#endif // ACTOR_H_
