#ifndef STRAINLINK_HPP
#define STRAINLINK_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"
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
        double update_position();
        void update_orientation();
        double getStrain();
        void resetLambda();
        b2Vec2 getVector();
        std::pair<int,int> getId();
        std::shared_ptr<particle> getParticleA();
        std::shared_ptr<particle> getParticleB();

    private:
        std::shared_ptr<particle> particle_a;
        std::shared_ptr<particle> particle_b;
        std::pair<int, int> id;
        double compliance;
        double normalized_compliance;
        double rest_length;
        double lambda;
};

inline StrainLink::StrainLink() {}

inline void StrainLink::resetLambda() {
    this->lambda = 0.;
}

inline double StrainLink::getStrain() {
    return log(this->getVector().Length() / this->rest_length);
}

#endif
