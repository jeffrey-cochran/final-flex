#include "particle.h"
#include "blob.hpp"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "box2d/box2d.h"
#include "params.hpp"


blob::blob(b2World& world,
           sf::Color color,
           int width,
           int height,
           float particles_per_unit_length,
           b2Vec2 center_of_mass)
{
    // TODO?
    height_discretization = height * particles_per_unit_length;
    width_discretization = width * particles_per_unit_length;
    
    //
    // Set center of mass
    position_.Set(center_of_mass.x, center_of_mass.y);
}

rectangle::rectangle(
	b2World& world,
	sf::Color color,
	int width, 
	int height, 
	float particles_per_unit_length, 
	b2Vec2 center_of_mass
) : blob(world, color, width, height, particles_per_unit_length, center_of_mass)
{
	//
	// Compute number of particles
	int num_particles = height_discretization * width_discretization;
	
	//
	// Get particle attributes
    float radius = 0.5 / (float)particles_per_unit_length;
    
	float current_x = center_of_mass.x - ((float)width / 2.) + radius;
	float current_y = center_of_mass.y + ((float)height / 2.) - radius;

	for( int i = 0; i < width_discretization; i++ ) {
		for( int j = 0; j < height_discretization; j++ ) {
            
            float new_x = current_x + (2.*radius*j);
            float new_y = current_y - (2.*radius*i);
            
 			//
			// Create next particle
			b2Vec2 center(
				new_x,
				new_y
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
    
    constructBVH();
}

bracket::bracket(b2World& world,
                 sf::Color color,
                 int width,
                 int height,
                 float particles_per_unit_length,
                 b2Vec2 center_of_mass,
                 float orientation,
                 float density)
: blob(world, color, width, height, particles_per_unit_length, center_of_mass)
{
    //
    // Compute number of particles
    num_particles = (height_discretization + width_discretization) - 1;

    //
    // Get particle attributes
    float radius = 0.5 / (float)particles_per_unit_length;
    
    float current_x = center_of_mass.x - ((float)width / 2.) + radius;
    float current_y = center_of_mass.y + ((float)height / 2.) - radius;

    for( int i = 0; i < width_discretization; i++ ) {
        
        float new_x = current_x;
        float new_y = current_y - (2.*radius*i);
        
        //std::cout << "new_x: " << new_x << std::endl;
        //std::cout << "new_y: " << new_y << std::endl;
        
        //
       // Create next particle
       b2Vec2 center(
           new_x,
           new_y
       );
        
        int index = i;
        //std::cout << "Index of particle: " << index << std::endl;
       particle p(
           radius,
           i,
           center,
           color,
           world
       );


       std::shared_ptr<particle> pp = std::make_shared<particle>(p);
       particles_.push_back(
           pp
       );
    }
    
    for( int j = 1; j < height_discretization; j++ ) {
        
        float new_x = current_x + (2.*radius*j);
        float new_y = current_y;
        
        //std::cout << "new_x: " << new_x << std::endl;
        //std::cout << "new_y: " << new_y << std::endl;
        
        //
       // Create next particle
       b2Vec2 center(
           new_x,
           new_y
       );
        int index = (width_discretization + j) - 1;
        //std::cout << "Index of particle: " << index << std::endl;
        particle p(
           radius,
           index,
           center,
           color,
           world
        );
        
        std::shared_ptr<particle> pp = std::make_shared<particle>(p);
        particles_.push_back(
           pp
        );
    }
    
    constructBVH();
    
}


void blob::solve_constraints() {

	//
	// Reset lambdas
	for( auto& link_pair : this->links_ ) {
		link_pair.second->resetLambda();
	}

    int constraint_iters = params::constraint_iters;
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
	float inv_time_step = params::inv_time_step;
	for( const auto p : this->particles_ ) {
		p->setLinearVelocity( inv_time_step * (p->getPosition() - p->getPreviousPosition()) );
	}

	//
	// Apply forces
	for( int m : this->forced_particles_ ) {
		this->getParticle(m)->applyForce();
	}



}

void blob::constructBVH() {
    // Construct a bounding volume hierarchy whose
    // leaf nodes are the particles of the blob
    sphere_bvh_ = BVH::TopDownSphereBVH(particles_);

    //
    // Generate strain-links within sphere of influence
    for( std::shared_ptr<particle> p : particles_ ) {

        SphereNode sphere_of_influence(p);
        sphere_of_influence.bounding_volume.radius *= 2.;
        auto li = sphere_bvh_->intersects(sphere_of_influence);

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

// This stays the same for the base class
void blob::fix(int k) {
	this->particles_[k]->invm = 0.;
	this->particles_[k]->setFixedPosition(this->particles_[k]->getPosition());
    this->fixed_particles_.emplace_back(k);
}

// This stays the same
void blob::applyForce(b2Vec2 force, int k) {
	this->particles_[k]->addForce(force);
	this->forced_particles_.emplace_back(k);
}

// This stays the same
void blob::fix(std::vector<int> kk) {
    this->fixed_particles_.reserve(this->fixed_particles_.size() + kk.size());
    this->fixed_particles_.insert(this->fixed_particles_.end(), kk.begin(),kk.end());
}

// This stays the same
std::shared_ptr<particle> blob::getParticle(int k) {
    return  this->particles_[k];
}

