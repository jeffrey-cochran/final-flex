#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"

/// A solid circle shape
class particle
{
    public:
        particle();
        particle(
            double radius, 
            int idx, 
            b2Vec2 pos, 
            sf::Color color,
            b2World&
        );
        void Draw(std::shared_ptr<sf::RenderWindow> main_window);
        void update();

    private:
        int idx;
        sf::CircleShape rendering_shape;
        b2Body* body;
        b2CircleShape physics_shape;

};

inline particle::particle(){
    this->idx = 0;
    physics_shape.m_p = b2Vec2(0.,0.);
    physics_shape.m_radius = 0.;
    body = nullptr;
}

inline particle::particle(
    double radius, 
    int idx, 
    b2Vec2 pos, 
    sf::Color color,
    b2World& world
) {
    idx = idx;

    physics_shape.m_p = pos;
    physics_shape.m_radius = radius;

    this->rendering_shape.setRadius(radius);
    this->rendering_shape.setPosition(pos.x, pos.y);
    this->rendering_shape.setFillColor(color);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    this->body = world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &physics_shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f; 
    fixtureDef.filter.groupIndex = -1;

    this->body->CreateFixture(&fixtureDef);

}

inline void particle::update(){
    if (this->body) {
        b2Vec2  position = this->body->GetPosition();
        this->rendering_shape.setPosition(position.x, position.y);
        printf("%4.2f %4.2f\n", position.x, position.y);
    }
    // 
}

#endif
