#include "particle.h"
#include "blob.hpp"
#include "StrainLink.hpp"
#include <vector>
#include <iostream>
#include "box2d/box2d.h"
#include "params.hpp"
#include "utils.hpp"

blob::blob(b2World& world,
           sf::Color color,
           b2Vec2 center_of_mass)
{
    
    //
    // Get particle attributes
    radius_ = 0.5 / (float)params::particles_per_unit_length;
    
    //
    // Set center of mass
    position_.Set(center_of_mass.x, center_of_mass.y);

    //
    // Increment body count
    utils::current_body_index++;
    this->id_ = utils::current_body_index;
}


segment::segment(b2World& world,
                 sf::Color color,
                 int c_radius,
                 float percentage,
                 b2Vec2 center_of_mass)
: blob(world, color, center_of_mass) {
    int radius = c_radius * params::particles_per_unit_length;
    std::cout << "Radius of cirlce (in little blobs): " << radius << std::endl;
    int c_length = std::round((2 * M_PI * radius));
    std::cout << "C_Length: " << c_length << std::endl;
}




void vnotch::fixCenter() {
    fix(center_);
}

void vnotch::fixLeftEdge() {
    for (int ind : left_edge_) {
        fix(ind);
    }
}

void vnotch::fixLeftTopEdge() {
    for (int ind : left_top_edge_) {
        fix(ind);
    }
}

void vnotch::fixLeftBottomEdge() {
    for (int ind : left_bottom_edge_) {
        fix(ind);
    }
    
}

void vnotch::fixRightTopEdge() {
    for (int ind : right_top_edge_) {
        fix(ind);
    }
}

void vnotch::applyRightBottomStrain(b2Vec2 strain) {
    for (int ind : right_bottom_edge_) {
        applyStrain(strain, ind);
    }
    
}

void vnotch::applyRightTopStrain(b2Vec2 strain) {
    for (int ind : right_top_edge_) {
        applyStrain(strain, ind);
    }
}

void vnotch::applyRightStrain(b2Vec2 strain) {
    applyRightTopStrain(strain);
    applyRightBottomStrain(strain);
}

void vnotch::fixRightBottomEdge() {
    for (int ind : right_bottom_edge_) {
        fix(ind);
    }
}
vnotch::vnotch(b2World& world,
    sf::Color color,
    int width,
    int height,
    int depth,
    b2Vec2 center_of_mass)
: blob(world, color, center_of_mass) {
    
    int height_discretization = height * params::particles_per_unit_length;
    int width_discretization = width * params::particles_per_unit_length;
    
    current_x_ = center_of_mass.x - ((float)width / 2.) + radius_;
    current_y_ = center_of_mass.y + ((float)height / 2.) - radius_;
    
    int particle_count = 0;
    
    std::map<int, int> depth_map;
    for (int w = 0; w < width_discretization; w++) {
        depth_map.insert(std::pair<int, int>(w, height_discretization));
    }
    
    int center_w = width_discretization / 2;
    
    int center_offset = 0;
    int d = height_discretization - 2*depth;
    for ( ; d <= height_discretization - 2; d += 2, center_offset++) {
        depth_map[center_w + center_offset] = d;
        depth_map[center_w - center_offset] = d;
    }
    
    for (int i = 0; i < width_discretization; i++) {
        int width_depth = depth_map.at(i);
        int w_d_offset = (width_depth - 1) / 2;
        for (int j = -w_d_offset; j <= w_d_offset; j++) {
            float new_x = current_x_ + (2.*radius_*i);
            float new_y = current_y_ - (2.*radius_*j);
            
             //
            // Create next particle
            b2Vec2 center(
                new_x,
                new_y
            );
            int index = particle_count;
            particle_count++;
            
            
            if (i == center_w && j == 0) {
                center_ = index;
            }
            
            if (i == 0) {
                left_edge_.push_back(index);
            }
            
            bool isLeft = i < center_w;
            bool isRight = i > center_w;
            bool isBottom = (j == w_d_offset);
            bool isTop = (j == -w_d_offset);
            bool isFull = (width_depth == height_discretization);
            
            if (isLeft && isBottom && isFull) {
                left_bottom_edge_.push_back(index);
            } else if (isLeft && isTop && isFull) {
                left_top_edge_.push_back(index);
            } else if (isRight && isBottom && isFull) {
                right_bottom_edge_.push_back(index);
            } else if (isRight && isTop && isFull) {
                right_top_edge_.push_back(index);
            }
            particle p(
                radius_,
                index,
                this->id_,
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

rectangle::rectangle(b2World& world,
                     sf::Color color,
                     int width,
                     int height,
                     b2Vec2 center_of_mass
) : blob(world, color, center_of_mass)
{
	//
	// Compute number of particles
    int height_discretization = height * params::particles_per_unit_length;
    int width_discretization = width * params::particles_per_unit_length;
	
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
                this->id_,
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
                 int thickness,
                 b2Vec2 center_of_mass)
: blob(world, color, center_of_mass)
{
    //
    // Compute number of particles
    int height_discretization = height * params::particles_per_unit_length;
    int width_discretization = width * params::particles_per_unit_length;
    int thickness_discretization = thickness * params::particles_per_unit_length;
    
    current_x_ = center_of_mass.x - ((float)width / 2.) + radius_;
    current_y_ = center_of_mass.y + ((float)height / 2.) - radius_;
    
    int particle_count = 0;
    int t_offset = (thickness_discretization - 1) / 2;
    for( int i = 0; i < width_discretization; i++ ) {
        for (int j = -t_offset; j <= t_offset; j++) {
            float new_x = current_x_ - (2.*radius_*j) - (2.*radius_*i);
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
               this->id_,
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
    
    for( int j = 1; j < height_discretization; j++ ) {
        for (int t = -t_offset; t <= t_offset; t++) {
            float new_x = current_x_ + (2.*radius_*j) - (2.*radius_*t);
            float new_y = current_y_ - (2.*radius_*j);
            
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
               this->id_,
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

void dogbone::fixTopShoulder() {
    for (int ind : top_shoulder_) {
        fix(ind);
    }
}

void dogbone::applyBottomStrain(b2Vec2 strain) {
    for (int ind : bottom_shoulder_) {
        applyStrain(strain, ind);
    }
}

dogbone::dogbone(b2World& world,
                 sf::Color color,
                 int shoulder_width,
                 int shoulder_height,
                 int neck_height,
                 int neck_width,
                 int transition_length,
                 b2Vec2 center_of_mass)
    : blob(world, color, center_of_mass) {
        
        //
        // Compute number of particles
        shoulder_width_ = shoulder_width * params::particles_per_unit_length;
        shoulder_height_ = shoulder_height * params::particles_per_unit_length;
        neck_height_ = neck_height * params::particles_per_unit_length;
        neck_width_ = neck_width * params::particles_per_unit_length;
        transition_length_ = transition_length * params::particles_per_unit_length;
        
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
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
                if (down_depth == target_depth - 1) {
                    bottom_shoulder_.push_back(index);
                }
                particle_count++;
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
                if (up_height == target_height) {
                    top_shoulder_.push_back(index);
                }
                particle_count++;
                
                particle p(radius_, index, this->id_, center, color, world);
                
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
		double fracture_strain = params::fracture_strain;
		double biggest_strain_proxy = 0.0;
		std::vector<std::pair<int,int>> links_with_excessive_strain;
		for( auto& link_pair : this->links_ ) {
			double strain_proxy = link_pair.second->update_position();
			biggest_strain_proxy = strain_proxy > biggest_strain_proxy ? strain_proxy : biggest_strain_proxy;
			if( strain_proxy > fracture_strain ) {
				links_with_excessive_strain.push_back(link_pair.first);
			}			
		}
        std::cout << biggest_strain_proxy << std::endl;

		//
		// Delete broken links
		bool deletion_occurs = false;
		if( links_with_excessive_strain.size() > 0 ) {
			for( auto& key : links_with_excessive_strain ) {

				//
				// Allow them to collide once separated
				this->links_.at(key)->getParticleA()->removeWhiteFlag(
                    this->id_,
                    this->links_.at(key)->getParticleB()->getIndex()
                );
				this->links_.at(key)->getParticleB()->removeWhiteFlag(
                    this->id_,
                    this->links_.at(key)->getParticleA()->getIndex()
                );

				this->links_.erase(key);
			}
			deletion_occurs = true;
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
        sphere_of_influence.bounding_volume.radius *= 2;
        auto li = sphere_bvh_->intersects(sphere_of_influence);

        for( int j : li ) {

            StrainLink current_link(p, this->getParticle(j));
            if( this->links_.count(current_link.getId()) == 0 ) {
                p->addWhiteFlag(this->id_, j);
                this->getParticle(
                    j
                )->addWhiteFlag(this->id_, p->getIndex());
                
                this->links_.insert(
                    { current_link.getId(), std::make_shared<StrainLink>(current_link) }
                );
            }

        }
    }
}

// This stays the same for the base class
void blob::fix(int k) {
    this->particles_[k]->setColor(sf::Color::Red);
	this->particles_[k]->invm = 0.;
	this->particles_[k]->setFixedPosition(this->particles_[k]->getPosition());
    this->fixed_particles_.emplace_back(k);
}

// This stays the same
void blob::applyForce(b2Vec2 force, int k) {
	this->particles_[k]->addForce(force);
	this->forced_particles_.emplace_back(k);
}

void blob::applyStrain(b2Vec2 strain_vector, int k) {
	this->particles_[k]->addStrain(strain_vector);
    this->particles_[k]->setColor(sf::Color::Cyan);
	this->fixed_particles_.emplace_back(k);
}

void blob::fix(std::vector<int> kk) {
    this->fixed_particles_.reserve(this->fixed_particles_.size() + kk.size());
    this->fixed_particles_.insert(this->fixed_particles_.end(), kk.begin(),kk.end());
}

// This stays the same
std::shared_ptr<particle> blob::getParticle(int k) {
    return  this->particles_[k];
}
