#include "scenario.hpp"
#include "fixture.hpp"

Scenario::Scenario(float particle_density,
                   float link_stiffness,
                   float damping_strength,
                   float radius_influence) {
    particle_density_ = particle_density;
    link_stiffness_ = link_stiffness;
    damping_strength_ = damping_strength;
    radius_influence_ = radius_influence;
}

DogboneStretch::DogboneStretch(int shoulder_width,
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
                               float radius_influence)
: Scenario(particle_density, link_stiffness, damping_strength, radius_influence),
bone_(world, sf::Color::White, shoulder_width, shoulder_height, neck_height, neck_width,
      transition_length, particle_density, center_of_mass)
{
    
    bone_.fixTopShoulder();
    bone_.applyBottomStrain(bottom_strain);
}

void DogboneStretch::run(std::shared_ptr<sf::RenderWindow> window) {
    bone_.update();
    bone_.solve_constraints();
    bone_.Draw(window);
}

