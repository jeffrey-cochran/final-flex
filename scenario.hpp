#ifndef SCENARIO_HPP
#define SCENARIO_HPP


#include "box2d/box2d.h"
#include "particle.h"
#include "SphereBVH.hpp"
#include "StrainLink.hpp"


class Scenario {
public:
    Scenario(float particle_density,
             float link_stiffness,
             float damping_strength,
             float radius_influence);
    
    virtual void setUp();
    
    virtual void evaluate(float time_step);
    
};

#endif
