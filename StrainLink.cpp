#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"
#include "utils.hpp"
#include "particle.h"
#include "StrainLink.hpp"
#include "params.hpp"


StrainLink::StrainLink(
    std::shared_ptr<particle> in_particle_a,
    std::shared_ptr<particle> in_particle_b
) {

    this->particle_a = in_particle_a->getIndex() < in_particle_b->getIndex() ? in_particle_a : in_particle_b;
    this->particle_b = in_particle_a->getIndex() < in_particle_b->getIndex() ? in_particle_b : in_particle_a;


    this->id = std::pair<int,int>(this->particle_a->getIndex(), this->particle_b->getIndex());
    
    this->rest_length = this->getVector().Length();

    this->lambda = 0;

    this->accumulated_plastic_strain = 0.;
}

b2Vec2 StrainLink::getVector() {
    return this->particle_a->getPosition() - this->particle_b->getPosition();
}

void StrainLink::update() {
    this->update_position();
}

double StrainLink::update_position() {

    double yield_strain = params::yield_strain;
    double time_step = params::time_step;
    double inv_time_step = params::inv_time_step;
    double normalized_strain_compliance = params::normalized_strain_compliance;
    // std::cout << normalized_strain_compliance << std::endl;
    
    double linear_damping = params::linear_damping;
    double normalized_beta = pow(time_step,2.) * linear_damping;
    double gamma = normalized_beta * normalized_strain_compliance * inv_time_step;

    //
    // Get magnitude and direction of perfect
    // correction to original length
    b2Vec2 direction = this->getVector();
    double magnitude = direction.Length() - this->rest_length;
    direction.Normalize();

    //
    // Update particle positions
    b2Vec2 cur_a = this->particle_a->getPosition();
    b2Vec2 cur_b = this->particle_b->getPosition();
    b2Vec2 prev_a = this->particle_a->getPreviousPosition();
    b2Vec2 prev_b = this->particle_b->getPreviousPosition();
    b2Vec2 delta = cur_a + cur_b - prev_a - prev_b;

    //
    // Compute the change in the lagrange multiplier
    double delta_lambda = -(
        magnitude + normalized_strain_compliance * this->lambda - gamma*(delta.x + delta.y)
    ) / (
        (1. + gamma) * (this->particle_a->invm + this->particle_b->invm) + normalized_strain_compliance
    );

    //
    // Update lagrange multiplier
    this->lambda += delta_lambda;

    //
    // Compute impulse for correction
    b2Vec2 impulse = delta_lambda * direction;
    // b2Vec2 new_a = prev_a + this->particle_a->invm * impulse;
    // if( particle_a->getIndex() == 2 ) {
    //     std::cout << prev_b.x <<", " << prev_b.y << std::endl;  
    //     std::cout << prev_a.x <<", " << prev_a.y << std::endl;   
    //     exit(0);
    // }
    this->particle_a->setPosition(
        cur_a + this->particle_a->invm * impulse 
    );
    this->particle_b->setPosition(
        cur_b - this->particle_b->invm * impulse 
    );

    //
    // Update plasticity
    double current_elastic_strain = this->getStrain();
    double current_plastic_strain = this->getPlasticStrain();
    double total_strain = current_elastic_strain + current_plastic_strain;
    double new_plastic_strain = current_elastic_strain - yield_strain;
    if( new_plastic_strain > 0) {
        this->accumulated_plastic_strain += new_plastic_strain;
        this->rest_length *= (1. + new_plastic_strain);
    }
    return total_strain;
};

std::pair<int,int> StrainLink::getId() {
    return this->id;
}


std::shared_ptr<particle> StrainLink::getParticleA() {
    return this->particle_a;
}

std::shared_ptr<particle> StrainLink::getParticleB() {
    return this->particle_b;
}
