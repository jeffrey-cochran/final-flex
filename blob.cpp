#include "particle.h"
#include "blob.hpp"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "box2d/box2d.h"
#include "params.hpp"


blob::blob(b2World& world,
           sf::Color color,
           float particles_per_unit_length,
           b2Vec2 center_of_mass)
{
    
    //
    // Get particle attributes
    radius_ = 0.5 / (float)particles_per_unit_length;
    
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
) : blob(world, color, particles_per_unit_length, center_of_mass)
{
	//
	// Compute number of particles
    int height_discretization = height * particles_per_unit_length;
    int width_discretization = width * particles_per_unit_length;
	
    current_x_ = center_of_mass.x - ((float)width / 2.) + radius_;
	current_y_ = center_of_mass.y + ((float)height / 2.) - radius_;

	for( int i = 0; i < width_discretization; i++ ) {
		for( int j = 0; j < height_discretization; j++ ) {
            
            float new_x = current_x_ + (2.*radius_*j);
            float new_y = current_y_ - (2.*radius_*i);
            
 			//
			// Create next particle
			b2Vec2 center(
				new_x,
				new_y
			);
			particle p(
				radius_,
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
    constructBVHAndLinks();
}

bracket::bracket(b2World& world,
                 sf::Color color,
                 int width,
                 int height,
                 float particles_per_unit_length,
                 b2Vec2 center_of_mass,
                 float orientation,
                 float density)
: blob(world, color, particles_per_unit_length, center_of_mass)
{
    //
    // Compute number of particles
    int height_discretization = height * particles_per_unit_length;
    int width_discretization = width * particles_per_unit_length;
    
    current_x_ = center_of_mass.x - ((float)width / 2.) + radius_;
    current_y_ = center_of_mass.y + ((float)height / 2.) - radius_;
    
    int particle_count = 0;

    for( int i = 0; i < width_discretization; i++ ) {
        
        float new_x = current_x_;
        float new_y = current_y_ - (2.*radius_*i);
        
        //
       // Create next particle
       b2Vec2 center(
           new_x,
           new_y
       );
        
        int index = particle_count;
        particle_count++;
        
       particle p(
           radius_,
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
    
    for( int j = 1; j < height_discretization; j++ ) {
        
        float new_x = current_x_ + (2.*radius_*j);
        float new_y = current_y_;
        
        //
       // Create next particle
       b2Vec2 center(
           new_x,
           new_y
       );
        int index = particle_count;
        particle_count++;
        
        particle p(
           radius_,
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
    
    constructBVHAndLinks();
    
}

void dogbone::fixTopShoulder() {
    for (int i = 1; i <= shoulder_width_; i++) {
        fix(num_particles_ - i);
    }
}

dogbone::dogbone(b2World& world,
                 sf::Color color,
                 int shoulder_width,
                 int shoulder_height,
                 int neck_height,
                 int neck_width,
                 int transition_length,
                 float particles_per_unit_length,
                 b2Vec2 center_of_mass)
    : blob(world, color, particles_per_unit_length, center_of_mass) {
        
        //
        // Compute number of particles
        shoulder_width_ = shoulder_width * particles_per_unit_length;
        shoulder_height_ = shoulder_height * particles_per_unit_length;
        neck_height_ = neck_height * particles_per_unit_length;
        neck_width_ = neck_width * particles_per_unit_length;
        transition_length_ = transition_length * particles_per_unit_length;
        
        int height = 2*shoulder_height_ + 2*transition_length_ + neck_height_;
        int width = shoulder_width_;
        
        current_x_ = center_of_mass.x - ((float)width / 2.) + radius_;
        current_y_ = center_of_mass.y + ((float)height / 2.) - radius_;
        
        int particle_count = 0;
        int down_depth = 0;
        int target_depth = std::ceil(neck_height_/2.0);
        int neck_offset = (neck_width_ - 1)/2.0;
        
        for ( ; down_depth < target_depth; down_depth++) {
            for (int j = -neck_offset; j<=neck_offset; j++) {
                
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ - (2.*radius_*down_depth);
                
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
        }
        
        target_depth = target_depth + transition_length_;
        int transition_width;
        for (transition_width = neck_width_; transition_width < shoulder_width_-2; transition_width += 2) {
            
        }
        
        int t_w_offset = (transition_width - 1) / 2;
        
        
        for ( ; down_depth < target_depth; down_depth++) {
            
            for (int j = -t_w_offset; j <= t_w_offset; j++) {
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ - (2.*radius_*down_depth);
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
            
        }
        
        target_depth += shoulder_height_;
        int s_w_offset = (shoulder_width_ - 1) / 2;
        for ( ; down_depth < target_depth; down_depth++) {
            for (int j = -s_w_offset; j <= s_w_offset; j++) {
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ - (2.*radius_*down_depth);
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
        }
        
        int target_height = neck_height_ / 2;
        
        int up_height = 1;
        for ( ; up_height <= target_height; up_height++) {
            for (int j = -neck_offset; j <= neck_offset; j++) {
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ + (2.*radius_*up_height);
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
            
        }
        
        target_height = target_height + transition_length_;
        
        
        for ( ; up_height <= target_height; up_height++) {
            
            for (int j = -t_w_offset; j <= t_w_offset; j++) {
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ + (2.*radius_*up_height);
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
            
        }
        
        target_height += shoulder_height_;
        for ( ; up_height <= target_height; up_height++) {
            for (int j = -s_w_offset; j <= s_w_offset; j++) {
                float new_x = current_x_ + (2.*radius_*j);
                float new_y = current_y_ + (2.*radius_*up_height);
                //
                // Create next particle
                b2Vec2 center(new_x, new_y);
                
                int index = particle_count;
                particle_count++;
                
                particle p(radius_, index, center, color, world);
                
                std::shared_ptr<particle> pp = std::make_shared<particle>(p);
                particles_.push_back(pp);
            }
        }
        constructBVHAndLinks();
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

int blob::num_particles() {
    return num_particles_;
}

void blob::constructBVHAndLinks() {
    
    num_particles_ = particles_.size();
    
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

