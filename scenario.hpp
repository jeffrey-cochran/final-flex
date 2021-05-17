#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "particle.h"
#include "SphereBVH.hpp"
#include "StrainLink.hpp"


class Scenario {
public:
    Scenario(float particle_density,
             float link_stiffness,
             float damping_strength,
             float radius_influence);
    
    void setUp();
    
    void evaluate(float time_step);
protected:
    float particle_density_;
    float link_stiffness_;
    float damping_strength_;
    float radius_influence_;
    
};

class DogboneStretch : public Scenario {
    DogboneStretch(int shoulder_width,
                   int shoulder_height,
                   int neck_height,
                   int neck_width,
                   int transition_length,
                   b2Vec2 bottom_strain,
                   b2World& world,
                   float particle_density,
                   float link_stiffness,
                   float damping_strength,
                   float radius_influence);
    
    //void setUp();
    
    //void evaluate(float time_step);
};

#endif
