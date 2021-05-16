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
    std::shared_ptr<particle> in_particle_b
) {

    this->particle_a = in_particle_a->getIndex() < in_particle_b->getIndex() ? in_particle_a : in_particle_b;
    this->particle_b = in_particle_a->getIndex() < in_particle_b->getIndex() ? in_particle_b : in_particle_a;


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

double StrainLink::update_position() {

    float time_step = params::time_step;
    float inv_time_step = params::inv_time_step;
    float normalized_strain_compliance = params::normalized_strain_compliance;
    float linear_damping = 10.;
    float normalized_beta = pow(time_step,2.) * linear_damping;
    float gamma = normalized_beta * normalized_strain_compliance * inv_time_step;

    //
    // Get magnitude and direction of perfect
    // correction to original length
    b2Vec2 direction = this->getVector();
    double magnitude = direction.Length() - this->rest_length;
    direction.Normalize();
    

	// double linear_damping = 0.0005;
	// for( auto& link_pair : this->links_ ) {
	// 	b2Vec2 prev_v_a =link_pair.second->getParticleA()->getLinearVelocity();
	// 	b2Vec2 prev_v_b =link_pair.second->getParticleB()->getLinearVelocity();
	// 	b2Vec2 velocity_impulse = (1./(link_pair.second->getParticleB()->invm + link_pair.second->getParticleA()->invm)) * (prev_v_b - prev_v_a);

	// 	link_pair.second->getParticleA()->setLinearVelocity( 
	// 		prev_v_a + linear_damping * velocity_impulse
	// 	);
	// 	link_pair.second->getParticleB()->setLinearVelocity( 
	// 		prev_v_b - linear_damping * velocity_impulse
	// 	);
	// }


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
    return this->getStrain();
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
