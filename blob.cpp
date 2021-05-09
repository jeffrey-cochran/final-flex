#include "particle.h"
#include "blob.hpp"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "Box2D/Box2D.h"
// #include "params.hpp"


blob::blob(
	b2World& world,
	sf::Color color,
	int width, 
	int height, 
	double particles_per_unit_length, 
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
	double radius = 0.5 / (double)particles_per_unit_length;
	double current_x = center_of_mass.x - ((double)width / 2.) + radius;
	double current_y = center_of_mass.y + ((double)height / 2.) - radius;

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

	//
	// Update the fixed position for
	// displacement controlled conditions
	for( const auto i : this->fixed_particles_ ) {
		this->particles_[i]->applyStrains();
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
		double strain_proxy_threshold = 0.5;
		double biggest_strain_proxy = 0.0;
		std::vector<std::pair<int,int>> links_with_excessive_strain;
		for( auto& link_pair : this->links_ ) {
			double strain_proxy = link_pair.second->update_position();
			biggest_strain_proxy = strain_proxy > biggest_strain_proxy ? strain_proxy : biggest_strain_proxy;
			if( strain_proxy > strain_proxy_threshold ) {
				links_with_excessive_strain.push_back(link_pair.first);
			}			
		}

		//
		// Delete broken links
		bool deletion_occurs = false;
		if( links_with_excessive_strain.size() > 0 ) {
			for( auto& key : links_with_excessive_strain ) {
				this->links_.erase(key);
			}
			deletion_occurs = true;
		}
		// if( deletion_occurs ) {
		// 	for( auto& p : this->particles_ ) {
		// 		p->clearStrains();
		// 	}
		// }
	}


	//
	// Update velocities
	double inv_time_step = 60000.;
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

void blob::applyStrain(b2Vec2 strain_vector, int k) {
	this->particles_[k]->addStrain(strain_vector);
	this->fixed_particles_.emplace_back(k);
}

void blob::fix(std::vector<int> kk) {
    this->fixed_particles_.reserve(this->fixed_particles_.size() + kk.size());
    this->fixed_particles_.insert(this->fixed_particles_.end(), kk.begin(),kk.end());
}

std::shared_ptr<particle> blob::getParticle(int k) {
	return  this->particles_[k];
}