#ifndef BLOB_HPP
#define BLOB_HPP

////////////////////////////////////////////////////////////
// Headers
////////////////////////////////////////////////////////////
#include <vector>
#include "Box2D/Box2D.h"
#include "particle.h"

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

#include <vector>
#include "Box2D/Box2D.h"
#include <SFML/Graphics.hpp>


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
	std::vector<particle>& particles();


//
// Attributes
private:
	b2Vec2 position;
    std::vector<particle> particles_;
};

inline blob::blob(
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
    particles_.resize(num_particles);
	
	//
	// Set center of mass
	position.Set(center_of_mass.x, center_of_mass.y);

	//
	// Get particle attributes
	float radius = 0.5 / (float)particles_per_unit_length;
	float current_x = center_of_mass.x - ((float)width / 2.) + radius;
	float current_y = center_of_mass.y + ((float)height / 2.) - radius;

	std::cout << radius << std::endl;

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
			particles_.emplace_back(p);
		}
	}  
}

inline void blob::update(){
	for (auto& p : particles_) {
		p.update();
	}
}

inline void blob::Draw(std::shared_ptr<sf::RenderWindow> main_window){
	for (auto& p : particles_) {
		p.Draw(main_window);
	}
}

#endif