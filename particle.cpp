#include "particle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void particle::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    main_window->draw(rendering_shape);
}


particle::particle(){
    this->setIndex(0);
    physics_shape.m_p = b2Vec2(0.,0.);
    physics_shape.m_radius = 0.;
    body = nullptr;
}

particle::particle(
    double radius, 
    int in_id, 
    b2Vec2 phys_pos, 
    sf::Color color,
    b2World& world
) {
    id = in_id;

    current_position.Set(phys_pos.x, phys_pos.y);
    previous_position.Set(phys_pos.x, phys_pos.y);
    fixed_position.Set(phys_pos.x, phys_pos.y);
    
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

        this->previous_position.Set(current_position.x, current_position.y);
        this->current_position = this->body->GetPosition();
        b2Vec2 vis_pos = utils::phys2vis(current_position);
        this->rendering_shape.setPosition(vis_pos.x, vis_pos.y);
    }
    // 
}

void particle::setPosition(double x, double y) {

    this->current_position.Set(x, y);

    b2Vec2 vis_position = utils::phys2vis(this->current_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(this->current_position, this->body->GetAngle());
}

void particle::setPosition(b2Vec2 new_position) {
    
    this->current_position.Set(new_position.x, new_position.y);

    b2Vec2 vis_position = utils::phys2vis(new_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(new_position, this->body->GetAngle());
}

void particle::setFixedPosition(b2Vec2 new_position) {
    this->fixed_position.Set(new_position.x, new_position.y);
}

void particle::setLinearVelocity(b2Vec2 new_velocity) {
    this->body->SetLinearVelocity(new_velocity);
}

b2Vec2 particle::getLinearVelocity() {
    return this->body->GetLinearVelocity();
}

b2Vec2 particle::getPosition() {
    return this->current_position;
}

b2Vec2 particle::getPreviousPosition() {
    return this->previous_position;
}

b2Vec2 particle::getFixedPosition() {
    return this->fixed_position;
}

void particle::setIndex(int index) {
    this->id = index;
}

int particle::getIndex() {
    return this->id;
}

void particle::addForce(b2Vec2 force) {
    this->forces.push_back(force);
}

void particle::applyStrains() {
    if( this->strains.size() > 0 ) {
        b2Vec2 new_fixed_position = this->getFixedPosition();
        for( const auto& strain : this->strains ) {
            new_fixed_position += strain;
        }
        this->setFixedPosition(new_fixed_position);
    }
    //
    // void
}

void particle::clearStrains() {
    this->strains.clear();
}

void particle::applyForce() {
    for( const auto& force : this->forces ) {
        this->body->ApplyForceToCenter(force, true);
    }
}