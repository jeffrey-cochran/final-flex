#include "fixture.hpp"


void fixture::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    main_window->draw(rendering_shape);
}

fixture::fixture(int index,
                 int width,
                 int height,
                 float orientation,
                 b2Vec2 pos,
                 sf::Color color,
                 b2World&)
{
    id = index;
    
    current_position.Set(pos.x, pos.y);
    previous_position.Set(pos.x, pos.y);
    fixed_position.Set(pos.x, pos.y);
    
    b2Vec2 vis_pos = utils::phys2vis(pos);
    
    physics_shape.SetAsBox(width/2., height/2., pos, orientation);
    this->rendering_shape.setSize(sf::Vector2f());
    //rendering_shape.setOrigin(radius, radius);
    this->rendering_shape.setFillColor(color);
    
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

void fixture::setFixedPosition(b2Vec2 new_position) {
    this->fixed_position.Set(new_position.x, new_position.y);
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

b2Vec2 fixture::getFixedPosition() {
    return this->fixed_position;
}

void fixture::setIndex(int index) {
    this->id = index;
}

int fixture::getIndex() {
    return this->id;
}

void fixture::addForce(b2Vec2 force) {
    this->forces.push_back(force);
}

void fixture::applyForce() {
    for( const auto& force : this->forces ) {
        this->body->ApplyForceToCenter(force, true);
    }
}
