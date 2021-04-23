#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"

/// A solid circle shape
class particle : public b2CircleShape
{
    public:
        particle();
        particle(
            double radius, 
            int idx, 
            b2Vec2 pos, 
            sf::Color color
        );
        void Draw(std::shared_ptr<sf::RenderWindow> main_window);
    private:
        int idx;
        sf::CircleShape shape;

};

inline particle::particle(){
    this->idx = 0;
    this->m_p = b2Vec2(0,0);
    this->m_radius = 0.;
}

inline particle::particle(
    double radius, 
    int idx, 
    b2Vec2 pos, 
    sf::Color color
) {
    this->idx = idx;
    this->m_p = pos;
    this->m_radius = radius;
    this->shape.setRadius(radius);
    this->shape.setPosition(pos.x, pos.y);
    this->shape.setFillColor(color);
}

#endif