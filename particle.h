#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "utils.hpp"

/// A solid circle shape
class particle
{
    public:
        particle();
        particle(
            double radius, 
            int index, 
            b2Vec2 pos, 
            sf::Color color,
            b2World&
        );
        void Draw(std::shared_ptr<sf::RenderWindow> main_window);
        void update();

        void setPosition(float x, float y);
        void setPosition(b2Vec2 new_position);
        void setFixedPosition(b2Vec2 new_position);
        b2Vec2 getPosition();
        b2Vec2 getPreviousPosition();
        b2Vec2 getFixedPosition();

        void setLinearVelocity(b2Vec2 new_velocity);
        b2Vec2 getLinearVelocity();

        float getRadius();

        void setColor(sf::Color c);
        void setIndex(int in_id);

        void applyForce();
        void addForce(b2Vec2 force);
        int getIndex();

        float invm;

    private:
        b2Body* body;
        sf::CircleShape rendering_shape;
        b2CircleShape physics_shape;
        b2Vec2 current_position, previous_position, fixed_position;
        b2Vec2 velocity;
        std::vector<b2Vec2> forces;
        int id;
};

inline float particle::getRadius(){
    return this->rendering_shape.getRadius();
}

inline void particle::setColor(sf::Color c) {
    this->rendering_shape.setFillColor(c);
}

#endif