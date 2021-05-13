#include "particle.h"
#include "blob.hpp"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "box2d/box2d.h"
#include "params.hpp"


blob::blob(
	b2World& world,
	sf::Color color,
	int width, 
	int height, 
	float particles_per_unit_length, 
	b2Vec2 center_of_mass
)
{
	//
	// Compute number of particles
	int height_discretization = height * particles_per_unit_length;
	int width_discretization = width * particles_per_unit_length;
	int num_particles = height_discretization * width_discretization;
	
	//
	// Set center of mass
	position.Set(center_of_mass.x, center_of_mass.y);

	//
	// Get particle attributes
	float radius = 0.5 / (float)particles_per_unit_length;
	float current_x = center_of_mass.x - ((float)width / 2.) + radius;
	float current_y = center_of_mass.y + ((float)height / 2.) - radius;

	for( int i = 0; i < width_discretization; i++ ) {
		for( int j = 0; j < height_discretization; j++ ) {

			//
			// Create next particle
			b2Vec2 center(
				current_x + (2.*radius*j), 
				current_y - (2.*radius*i)
			);
			particle p(
				radius, 
				i*height_discretization + j, 
				center,
				color,
				world
			);


			std::shared_ptr<particle> pp = std::make_shared<particle>(p);
			particles_.push_back(
				pp
			);
		}
	}  

	//
	// Construct a bounding volume hierarchy whose
	// leaf nodes are the particles of the blob
	sphere_bvh = BVH::TopDownSphereBVH(particles_);

	//
	// Generate strain-links within sphere of influence
	for( std::shared_ptr<particle> p : particles_ ) {

		SphereNode sphere_of_influence(p);
		sphere_of_influence.bounding_volume.radius *= 2.;
		auto li = sphere_bvh->intersects(sphere_of_influence);

		for( int j : li ) {

            StrainLink current_link(p, this->getParticle(j));
            if( this->links_.count(current_link.getId()) == 0 ) {
                this->links_.insert(
                    { current_link.getId(), std::make_shared<StrainLink>(current_link) }
                );
            }

		}
	}
}

void blob::solve_constraints() {

	//
	// Reset lambdas
	for( auto& link_pair : this->links_ ) {
		link_pair.second->resetLambda();
	}

	int constraint_iters = 1;
	for( int i = 0; i < constraint_iters; i++ ) {
 
		//
		// Solve fixture constraints
		for( const auto i : this->fixed_particles_ ) {
			b2Vec2 original_position = this->particles_[i]->getFixedPosition();
			this->particles_[i]->setPosition(
				original_position.x, original_position.y
			);
		}

		//
		// Solve link constraints
		for( auto& link_pair : this->links_ ) {
			link_pair.second->update_position();
		}
	}


	//
	// Update velocities
	float inv_time_step = 600.;
	for( const auto p : this->particles_ ) {
		p->setLinearVelocity( inv_time_step * (p->getPosition() - p->getPreviousPosition()) );
	}

	//
	// Apply forces
	for( int m : this->forced_particles_ ) {
		this->getParticle(m)->applyForce();
	}



}

void blob::fix(int k) {
	this->particles_[k]->invm = 0.;
	this->particles_[k]->setFixedPosition(this->particles_[k]->getPosition());
    this->fixed_particles_.emplace_back(k);
}

void blob::applyForce(b2Vec2 force, int k) {
	this->particles_[k]->addForce(force);
	this->forced_particles_.emplace_back(k);
}

void blob::fix(std::vector<int> kk) {
    this->fixed_particles_.reserve(this->fixed_particles_.size() + kk.size());
    this->fixed_particles_.insert(this->fixed_particles_.end(), kk.begin(),kk.end());
}

std::shared_ptr<particle> blob::getParticle(int k) {
	return  this->particles_[k];
}
