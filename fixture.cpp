#include "fixture.hpp"
#include "params.hpp"
#include <iostream>


void fixture::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    main_window->draw(rendering_shape);
}

fixture::fixture(int index,
                 float width,
                 float height,
                 float orientation,
                 b2Vec2 pos,
                 sf::Color color,
                 b2World& world)
{
    id = index;
    
    current_position.Set(pos.x, pos.y);
    previous_position.Set(pos.x, pos.y);
    
    b2Vec2 vis_pos = utils::phys2vis(pos);
    
    float hx = width/2.0;
    float hy = height/2.0;
    
    b2Vec2 v1(142.5, 92.5);
    b2Vec2 v2(157.5, 92.5);
    b2Vec2 v3(157.5, 107.5);
    b2Vec2 v4(142.5, 107.5);
    
    b2Vec2 vertices[4];
    vertices[0] = v1;
    vertices[1] = v2;
    vertices[2] = v3;
    vertices[3] = v4;
    std::cout << "Num of vertices: " << sizeof(vertices) << std::endl;
    
    
    this->physics_shape.SetAsBox(width/2.f, height/2.f, pos, orientation); // Setting as this for now
    
    
    for (b2Vec2 v : this->physics_shape.m_vertices) {
        std::cout << v.x << ", " << v.y << std::endl;
    }
    this->rendering_shape.setSize(sf::Vector2f(width, height));
    this->rendering_shape.setPosition(pos.x, pos.y);
    this->rendering_shape.setRotation(orientation);
    this->rendering_shape.setFillColor(color);
    
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(pos.x, pos.y);
    this->body = world.CreateBody(&bodyDef);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &physics_shape;
    fixtureDef.density = params::density;
    fixtureDef.friction = params::friction;
    fixtureDef.filter.groupIndex = 1;

    this->body->CreateFixture(&fixtureDef);

    this->invm = 1./this->body->GetMass();
    
}

void fixture::updateVelocity() {
    b2Vec2 check = getPosition() - getPreviousPosition();
    setLinearVelocity(params::inv_time_step * (check));
}

void fixture::update(){
    if (this->body) {

        this->previous_position.Set(current_position.x, current_position.y);
        this->current_position = this->body->GetPosition();
        b2Vec2 vis_pos = utils::phys2vis(current_position);
        this->rendering_shape.setPosition(vis_pos.x, vis_pos.y);
    }
    //
}

void fixture::setPosition(double x, double y) {

    this->current_position.Set(x, y);

    b2Vec2 vis_position = utils::phys2vis(this->current_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(this->current_position, this->body->GetAngle());
}

void fixture::setPosition(b2Vec2 new_position) {
    
    this->current_position.Set(new_position.x, new_position.y);

    b2Vec2 vis_position = utils::phys2vis(new_position);

    this->rendering_shape.setPosition(vis_position.x, vis_position.y);
    this->body->SetTransform(new_position, this->body->GetAngle());
}

void fixture::setLinearVelocity(b2Vec2 new_velocity) {
    this->body->SetLinearVelocity(new_velocity);
}

b2Vec2 fixture::getLinearVelocity() {
    return this->body->GetLinearVelocity();
}

b2Vec2 fixture::getPosition() {
    return this->current_position;
}

b2Vec2 fixture::getPreviousPosition() {
    return this->previous_position;
}

void fixture::setIndex(int index) {
    this->id = index;
}

int fixture::getIndex() {
    return this->id;
}

void fixture::addForce(b2Vec2 force) {
    std::cout << "added force" << std::endl;
    this->forces.push_back(force);
}

void fixture::applyForce() {
    for( const auto& force : this->forces ) {
        this->body->ApplyForceToCenter(force, true);
    }
}
