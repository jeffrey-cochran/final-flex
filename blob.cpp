#include "blob.hpp"
#include "particle.h"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "Box2D/Box2D.h"


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

	int cnt = 0;

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
	for( auto p : particles_ ) {

		SphereNode sphere_of_influence(p);
		sphere_of_influence.bounding_volume.radius *= 2.;
		auto li = sphere_bvh->intersects(sphere_of_influence);

		for( int j : li ) {

            StrainLink current_link(p, this->getParticle(j);
            if this->links_.count(current_link.getId() == 0) {
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
	for( const auto link : this->links_ ) {
		link->resetLambda();
	}

	for( int i = 0; i < params::constraint_iters; i++ ) {

		//
		// Solve fixture constraints
		for( const auto i : this->fixed_paricles_ ) {
			b2Vec2 original_position = this->particles_[i]->getOriginalPosition();
			this->particles_[i]->setPosition(
				original_position.x, original_position.y
			);
		}

		//
		// Solve link constraints
		for( const auto link : this->links_ ) {
			link->update_position();
		}
	}

	//
	// Update velocities
	for( const auto p : this->particles_ ) {
		p->setLinearVelocity( params::inv_time_step * (p->getPosition() - p->getPreviousPosition());
	}
}

void blob::fix(int k) {
    this->fixed_paricles_.emplace_back(k);
}

void blob::fix(std::vector<int> kk) {
    this->fixed_paricles_.reserve(this->fixed_paricles_.size() + kk.size());
    this->fixed_paricles_.insert(this->fixed_paricles_.end(), kk.begin(),kk.end());
}