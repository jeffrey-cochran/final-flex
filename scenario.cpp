#include "scenario.hpp"
#include "blob.hpp"
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
                               b2Vec2 bottom_strain,
                               b2World& world,
                               float particle_density,
                               float link_stiffness,
                               float damping_strength,
                               float radius_influence)
: Scenario(particle_density, link_stiffness, damping_strength, radius_influence)
{
    b2Vec2 center_of_mass(100., 120.);
    dogbone bone(world, sf::Color::White, 35, 20, 55, 15, 10, 0.2, center_of_mass);
    bone.fixTopShoulder();
    bone.applyBottomStrain(bottom_strain);
}

