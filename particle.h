#ifndef PARTICLE_HPP
#define PARTICLE_HPP

#include <vector>
#include <set>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "Box2D/Box2D.h"
#include "utils.hpp"

struct particle_data {
    particle_data();
    std::shared_ptr<std::set<int>> w_flags;
    int p_index;
};

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

        void setPosition(double x, double y);
        void setPosition(b2Vec2 new_position);
        void setFixedPosition(b2Vec2 new_position);
        b2Vec2 getPosition();
        b2Vec2 getPreviousPosition();
        b2Vec2 getFixedPosition();

        void setLinearVelocity(b2Vec2 new_velocity);
        b2Vec2 getLinearVelocity();

        double getRadius();

        void setColor(sf::Color c);
        void setIndex(int in_id);

        void applyForce();
        void applyStrains();
        void addForce(b2Vec2 force);
        void addStrain(b2Vec2 strain_vector);
        int getIndex();

        void clearStrains();

        void addWhiteFlag(int k);
        void removeWhiteFlag(int k);

        double invm;

    private:
        b2Body* body;
        sf::CircleShape rendering_shape;
        b2CircleShape physics_shape;
        b2Vec2 current_position, previous_position, fixed_position;
        b2Vec2 velocity;
        std::vector<b2Vec2> forces;
        std::vector<b2Vec2> strains;
        std::set<int> white_flags;
        int id;
};

inline double particle::getRadius(){
    return this->rendering_shape.getRadius();
}

inline void particle::setColor(sf::Color c) {
    this->rendering_shape.setFillColor(c);
}

inline void particle::addStrain(b2Vec2 strain_vector) {
    this->strains.push_back(strain_vector);
}

inline void particle::addWhiteFlag(int k) {
    this->white_flags.insert(k);
}

inline void particle::removeWhiteFlag(int k) {
    this->white_flags.erase(k);
}

#endif
