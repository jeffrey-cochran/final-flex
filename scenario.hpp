#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "particle.h"
#include "blob.hpp"
#include "SphereBVH.hpp"
#include "StrainLink.hpp"
#include "fixture.hpp"


class Scenario {
public:
    Scenario(float particle_density,
             float link_stiffness,
             float damping_strength,
             float radius_influence);
    
    
    virtual void run(std::shared_ptr<sf::RenderWindow> window) = 0;
   
    
    void evaluate(float time_step);
protected:
    float particle_density_;
    float link_stiffness_;
    float damping_strength_;
    float radius_influence_;
    
};

class DogboneStretch : public Scenario {
public:
    DogboneStretch(int shoulder_width,
                   int shoulder_height,
                   int neck_height,
                   int neck_width,
                   int transition_length,
                   b2Vec2 center_of_mass,
                   b2Vec2 bottom_strain,
                   b2World& world,
                   float particle_density,
                   float link_stiffness,
                   float damping_strength,
                   float radius_influence);
    
    void run(std::shared_ptr<sf::RenderWindow> window);
    
private:
    dogbone bone_;
};

class VNotchBreak : public Scenario {
public:
    VNotchBreak(int vn_width,
                int vn_height,
                int n_depth,
                int f_width,
                int f_heigth,
                b2Vec2 vn_center,
                b2Vec2 f_center,
                b2World& world,
                b2Vec2 f_force,
                float pd,
                float ls,
                float ds,
                float ri);
    
    void run(std::shared_ptr<sf::RenderWindow> window);
private:
    vnotch vn_;
    fixture rect_;
};

#endif
