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
    b2Vec2 phys_pos, 
    sf::Color color,
    b2World& world
) {
    idx = idx;
    b2Vec2 vis_pos = utils::phys2vis(phys_pos);

    // physics_shape.m_p = phys_pos;
    physics_shape.m_radius = radius;
    
    this->rendering_shape.setRadius(radius);
    rendering_shape.setOrigin(radius, radius);
    this->rendering_shape.setPosition(vis_pos.x, vis_pos.y);
    this->rendering_shape.setFillColor(color);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(phys_pos.x, phys_pos.y);
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
        b2Vec2  phys_pos = this->body->GetPosition();
        b2Vec2  vis_pos = utils::phys2vis(phys_pos);
        this->rendering_shape.setPosition(vis_pos.x, vis_pos.y);
        // b2Vec2  mp = this->body->GetLocalCenter();
        // printf("Vis Pos: %4.2f %4.2f\n", vis_pos.x, vis_pos.y);
        // printf("Phys Pos: %4.2f %4.2f\n", phys_pos.x, phys_pos.y);
    }
    // 
}

#endif