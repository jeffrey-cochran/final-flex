#ifndef STRAINLINK_HPP
#define STRAINLINK_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "utils.hpp"
#include "particle.h"

/// A solid circle shape
class StrainLink
{
    public:
        StrainLink();
        StrainLink(
            std::shared_ptr<particle> in_particle_a,
            std::shared_ptr<particle> in_particle_b
        );

        void update();
        float update_position();
        void update_orientation();
        float getStrain();
        void resetLambda();
        b2Vec2 getVector();
        std::pair<int,int> getId();
        std::shared_ptr<particle> getParticleA();
        std::shared_ptr<particle> getParticleB();

    private:
        std::shared_ptr<particle> particle_a;
        std::shared_ptr<particle> particle_b;
        std::pair<int, int> id;
        float compliance;
        float normalized_compliance;
        float rest_length;
        float lambda;
};

inline StrainLink::StrainLink() {}

inline void StrainLink::resetLambda() {
    this->lambda = 0.;
}

inline float StrainLink::getStrain() {
    return log(this->getVector().Length() / this->rest_length);
}

#endif