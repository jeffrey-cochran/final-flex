#ifndef SCENARIO_HPP
#define SCENARIO_HPP

#include "particle.h"
#include "blob.hpp"
#include "SphereBVH.hpp"
#include "StrainLink.hpp"
#include "fixture.hpp"
#include "ScenarioData.hpp"


class Scenario {
public:
    Scenario();
    virtual void run(std::shared_ptr<sf::RenderWindow> window) = 0;
    void evaluate(float time_step);
};

class DogboneStretch : public Scenario {
public:
    DogboneStretch(struct ScenarioData* data, b2World& world);
    void run(std::shared_ptr<sf::RenderWindow> window);
    
private:
    dogbone bone_;
};

class VNotchBreak : public Scenario {
public:
    VNotchBreak(struct ScenarioData* data, b2World& world);
    
    void run(std::shared_ptr<sf::RenderWindow> window);
private:
    vnotch vn_;
};

class LBracketBreak : public Scenario {
public:
    LBracketBreak(struct ScenarioData* data, b2World& world);
    void run(std::shared_ptr<sf::RenderWindow> window);
    
private:
    bracket lbracket_;
    fixture b1_, b2_, b3_, b4_;
};

#endif
