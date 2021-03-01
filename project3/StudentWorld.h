#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "GameWorld.h"
#include "Actor.h"
#include <string>
#include <vector>

class StudentWorld : public GameWorld
{
public:
    StudentWorld(std::string assetPath);
    ~StudentWorld();
    virtual int init();
    virtual int move();
    virtual void cleanUp();
    GhostRacer* getghostracer() const;
    int get_soul_saved() const;
    void save_a_soul();
    int get_bonus_score() const;
    void set_bonus_score(int score);
    std::vector<Actor*> get_m_actor();
    void add_healing_goodie(double x_cor, double y_cor);
    void add_oil_slick(double x_cor, double y_cor);
    void add_holy_water_projectile(double x_cor, double y_cor, int dir);
private:
    GhostRacer* m_ghostracer;
    std::vector<Actor*> m_actor;
    int soul_saved;
    int bonus_score;
    bool level_end;
    //auxillary private function
    void add_borderline();
    void add_human_ped();
    void add_zombie_ped();
    void add_zombie_cab();
    void add_oil_slick();
    void add_holy_water_goodie();
    void add_soul_goodie();
    double get_lane_x_coordinate(int n) const;
};

#endif // STUDENTWORLD_H_
