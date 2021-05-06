#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "utils.hpp"
#include "particle.h"
#include "StrainLink.hpp"
#include "params.hpp"


StrainLink::StrainLink(
    std::shared_ptr<particle> in_particle_a,
    std::shared_ptr<particle> in_particle_b,
    float in_compliance,
    float time_step
) {
    this->particle_a = in_particle_a->getIndex() < in_particle_b->getIndex() ? particle_a : particle_b;
    this->particle_b = in_particle_a->getIndex() < in_particle_b->getIndex() ? particle_b : particle_a;

    this->id = std::make_pair<int,int>(this->particle_a->getIndex(), this->particle_b->getIndex());
    
    this->rest_length = this->getVector().Length();

    this->lambda = 0;
}

b2Vec2 StrainLink::getVector() {
    return this->particle_a->getPosition() - this->particle_b->getPosition();
}

void StrainLink::update() {
    this->update_position();
}

void StrainLink::update_position() {

    //
    // Get magnitude and direction of perfect
    // correction to original length
    b2Vec2 direction = this->getVector();
    float magnitude = direction.Length() - this->rest_length;
    direction.Normalize();
    
    //
    // Compute the change in the lagrange multiplier
    float delta_lambda = -(
        magnitude + params::normalized_strain_compliance * this->lambda
    ) / (
        this->particle_a->invm + this->particle_b->invm + params::normalized_strain_compliance
    );

    //
    // Update lagrange multiplier
    this->lambda += delta_lambda;

    //
    // Compute impulse for correction
    b2Vec2 impulse = delta_lambda * direction;

    //
    // Update particle positions
    b2Vec2 prev_a = this->particle_a->getPosition();
    b2Vec2 prev_b = this->particle_a->getPosition();
    this->particle_a->setPosition(
        prev_a + this->particle_a->invm * impulse 
    );
    this->particle_b->setPosition(
        prev_b - this->particle_b->invm * impulse 
    );

    //
    // void
};

std::pair<int,int> StrainLink::getId() {
    return this->id;
}