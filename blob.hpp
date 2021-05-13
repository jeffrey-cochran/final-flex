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
		int width, 
		int height, 
		float particles_per_unit_length, 
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
	std::vector<particle> particles();
	std::shared_ptr<particle> getParticle(int);

	void fix(int k);
	void fix(std::vector<int> kk);

	void applyForce(b2Vec2 force, int k);
	void applyForce(b2Vec2 force, std::vector<int> kk);

	void setColor(sf::Color);

	void solve_constraints();

//
// Attributes
private:
	b2Vec2 position;
    std::vector<std::shared_ptr<particle>> particles_;
	std::map<std::pair<int, int>, std::shared_ptr<StrainLink>> links_;
	std::vector<int> fixed_particles_;
	std::vector<int> forced_particles_;
	Eigen::VectorXd lambdas;
	Eigen::SparseMatrix<double> inv_mass;
	float isotropic_stiffness;
	std::shared_ptr<SphereNode> sphere_bvh;
	
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
