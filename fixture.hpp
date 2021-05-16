#ifndef FIXTURE_HPP
#define FIXTURE_HPP

#include <vector>
#include <SFML/Graphics.hpp>
#include "utils.hpp"

/// A solid rectangle shape
class fixture {
public:
    fixture(int index,
            int width,
            int height,
            float orientation,
            b2Vec2 pos,
            sf::Color color,
            b2World&);
    
    void Draw(std::shared_ptr<sf::RenderWindow> main_window);
    void update();
    
    void setLinearVelocity(b2Vec2 new_velocity);
    b2Vec2 getLinearVelocity();
    
    void setColor(sf::Color c);
    void setIndex(int in_id);

    void applyForce();
    void addForce(b2Vec2 force);
    int getIndex();
    
    void setPosition(double x, double y);
    void setPosition(b2Vec2 new_position);
    void setFixedPosition(b2Vec2 new_position);
    
    b2Vec2 getPosition();
    b2Vec2 getPreviousPosition();
    b2Vec2 getFixedPosition();
    
private:
    b2Body* body;
    sf::RectangleShape rendering_shape;
    b2PolygonShape physics_shape;
    b2Vec2 current_position, previous_position, fixed_position;
    b2Vec2 velocity;
    std::vector<b2Vec2> forces;
    int id;
};

inline void fixture::setColor(sf::Color c) {
    this->rendering_shape.setFillColor(c);
}

#endif
