#include "particle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void particle::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    main_window->draw(rendering_shape);
}


inline particle::particle(){
    this->setIndex(0);
    physics_shape.m_p = b2Vec2(0.,0.);
    physics_shape.m_radius = 0.;
    body = nullptr;
}

inline particle::particle(
    double radius, 
    int idx, 
    b2Vec2 phys_pos, 
    sf::Color color,
    b2World& world,
    float time_step
) {
    this->setIndex(idx);

    current_position = phys_pos;
    previous_position = phys_pos;
    original_position = phys_pos;
    
    b2Vec2 vis_pos = utils::phys2vis(phys_pos);

    // physics_shape.m_p = phys_pos;
    physics_shape.m_radius = radius;
    
    this->rendering_shape.setRadius(radius);
    rendering_shape.setOrigin(radius, radius);
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

    this->invm = 1./this->body->GetMass();

}

void particle::update(){
    if (this->body) {

        b2Vec2  previous_position = current_position;
        b2Vec2  current_position = this->body->GetPosition();
        b2Vec2  vis_pos = utils::phys2vis(current_position);
        this->rendering_shape.setPosition(vis_pos.x, vis_pos.y);
    }
    // 
}

void particle::setPosition(float x, float y) {
    
    b2Vec2 phys_position(x, y);
    b2Vec2 vis_position = utils::phys2vis(phys_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(phys_position, this->body->GetAngle());
}

void particle::setPosition(b2Vec2 new_position) {
    
    b2Vec2 vis_position = utils::phys2vis(new_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(new_position, this->body->GetAngle());
}

void particle::setLinearVelocity(b2Vec2 new_velocity) {
    this->body->SetLinearVelocity(new_velocity);
}

b2Vec2 particle::getPosition() {
    return this->current_position;
}

b2Vec2 particle::getPreviousPosition() {
    return this->previous_position;
}

b2Vec2 particle::getOriginalPosition() {
    return this->original_position;
}
