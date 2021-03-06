#ifndef BLOB_HPP
#define BLOB_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include "box2d/box2d.h"
#include "particle.h"
#include "SphereBVH.hpp"
#include "StrainLink.hpp"
#include <SFML/Graphics.hpp>
#include <Eigen/Dense>
#include <Eigen/Sparse>

// MIT License

// Copyright (c) 2019 Erin Catto

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

class blob
{
//
// Methods
public:

	//
	// Constructors
	blob(
		b2World& world,
		sf::Color color,
		b2Vec2 center_of_mass
	);

	//
	// Visualization
	void Draw(std::shared_ptr<sf::RenderWindow> main_window);

	//
	// Dynamics
	void update();

	//
	// Attribute Access
    int num_particles();
	std::vector<particle> particles();
	std::shared_ptr<particle> getParticle(int);

	void fix(int k);
	void fix(std::vector<int> kk);

	void applyForce(b2Vec2 force, int k);
	void applyForce(b2Vec2 force, std::vector<int> kk);

	void applyStrain(b2Vec2 strain_vector, int k);

	void setColor(sf::Color);

	void solve_constraints();
    
    void constructBVHAndLinks();

//
// Attributes
protected:
	b2Vec2 position_;
    std::vector<std::shared_ptr<particle>> particles_;
	std::map<std::pair<int, int>, std::shared_ptr<StrainLink>> links_;
	std::vector<int> fixed_particles_;
	std::vector<int> forced_particles_;
	Eigen::VectorXd lambdas_;
	Eigen::SparseMatrix<double> inv_mass_;
	float isotropic_stiffness;
	std::shared_ptr<SphereNode> sphere_bvh_;
    
    int num_particles_;
    int id_;
    float radius_;
    float current_x_;
    float current_y_;
	
};

class rectangle : public blob {
public:
    rectangle(b2World& world,
              sf::Color color,
              int width,
              int height,
              b2Vec2 center_of_mass);
};

class bracket : public blob {
public:
    bracket(b2World& world,
            sf::Color color,
            int width,
            int height,
            int thickness,
            b2Vec2 center_of_mass);
};

class dogbone : public blob {
public:
    dogbone(b2World& world,
            sf::Color color,
            int shoulder_width,
            int shoulder_height,
            int neck_height,
            int neck_width,
            int transition_length,
            b2Vec2 center_of_mass);
    
    void fixTopShoulder();
    
    void applyBottomStrain(b2Vec2 strain);
    
private:
    int shoulder_width_;
    int shoulder_height_;
    int neck_height_;
    int neck_width_;
    int transition_length_;
    
    std::vector<int> top_shoulder_;
    std::vector<int> bottom_shoulder_;
};

class vnotch : public blob {
public:
    vnotch(b2World& world,
           sf::Color color,
           int width,
           int height,
           int depth,
           b2Vec2 center_of_mass);
    
    void fixCenter();
    void fixLeftEdge();
    void fixLeftTopEdge();
    void fixLeftBottomEdge();
    
    void applyRightStrain(b2Vec2 strain);
    void applyRightBottomStrain(b2Vec2 strain);
    void applyRightTopStrain(b2Vec2 strain);
    void fixRightTopEdge();
    void fixRightBottomEdge();
    void fixRightEdge();
private:
    int center_;
    // could potentially come in handy to fix the four corners
    std::vector<int> corners_;
    std::vector<int> left_edge_;
    std::vector<int> left_top_edge_;
    std::vector<int> left_bottom_edge_;
    std::vector<int> right_top_edge_;
    std::vector<int> right_bottom_edge_;
    std::vector<int> right_edge_;
};

class segment : public blob {
public:
    segment(b2World& world,
            sf::Color color,
            int c_radius,
            float percentage,
            b2Vec2 center_of_mass);
};

inline void blob::update(){
	for (auto& p : particles_) {
		assert(p);
		p->update();
	}
}

inline void blob::Draw(std::shared_ptr<sf::RenderWindow> main_window){

	for (auto& p : particles_) {
		assert(p);
		p->Draw(main_window);
	}
}


#endif
