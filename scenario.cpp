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
/**
 shoulder_width: width of the dogbone shoulder (usually 35, 45, 55)
 shoulder_height: height of the dogbone shoulder (usually 25, 35)
 neck_height: height of the neck of the dogbone (usually 55)
 neck_width: width of the neck of the dogbone (usually 15, 25)
 transition_length: length of transition between neck and shoulder (usually 2, 3) (still working on the sloping transition)
 center_of_mass: center of mass for the dogbone itself
 bottom_strain: strain to be applied to the bottom particles
 particle_density: density of particles (usually 0.2, 0.4, 0.6)
 link_stiffness: not yet included
 damping_stiffness: not yet included
 radius_influence: not yet included
 */
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

/**
 vn_width: width of the vnotch block
 vn_height: height of the vnotch block
 n_depth: depth of the vnotch itself (usually around 1 or 2)
 f_width: the width of the box to be pushed down
 f_height: the height of the box to be pushed down
 vn_center: center of mass for the vnotch block
 f_center: center of mass for the block to be pushed down
 f_force: force (or eventually displacement) to be added to the block
 pd: particle_density (usually 0.2)
 ls: link stiffness (not yet implemented)
 ds: damping_stiffness (not yet implemented)
 ri: radius influence (not yet implemented)
 
 
 This will apply a force to the block, but we don't want a force, but a strain like displacement
 like how we did with fixed particles in blob. For some reason, I can't get the box to collide if I
 specify a displacement.
 
 Take a look at addDisplacement and applyDisplacement in fixture.cpp
 */
VNotchBreak::VNotchBreak(int vn_width, int vn_height, int n_depth, int f_width,
                         int f_heigth, b2Vec2 vn_center, b2Vec2 f_center, b2World& world,
                         b2Vec2 f_force, float pd, float ls, float ds, float ri)
: Scenario(pd, ls, ds, ri), vn_(world, sf::Color::White, vn_width, vn_height, n_depth, pd, vn_center),
rect_(0, f_width, f_heigth, 0.0, f_center, sf::Color::Green, world)
{
    vn_.fixLeftEdge();
    // rect_.addForce(f_force);
    rect_.addDisplacement(f_force);
}

void VNotchBreak::run(std::shared_ptr<sf::RenderWindow> window) {
    vn_.update();
    vn_.solve_constraints();
    vn_.Draw(window);
    
    rect_.update();
    // rect_.applyForce();
    rect_.applyDisplacement();
    rect_.Draw(window);
};

