#include <SFML/Graphics.hpp>
#include "blob.hpp"
#include "particle.h"
#include "Box2D/Box2D.h"
#include <iostream>


int main()
{
    //
    // Create world
    b2Vec2 gravity(0.0f, -0.1f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, -10.0f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(500.0f, 10.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    sf::RectangleShape ground; 
    sf::Vector2f pos(0.0f, 197.0f);
    sf::Vector2f size(1000.0, 1.0);
    ground.setSize(size);
    ground.setPosition(pos);
    ground.setOutlineColor(sf::Color::White);

    //
    // Create visualizer window
    std::shared_ptr<sf::RenderWindow> main_window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(200, 200), 
        "SFML works!"
    );


    b2Vec2 center_of_mass(50., 100.);
    //
    // Create a bunch of particles
    blob my_blob(
        world,
        sf::Color::Magenta,
        30, 
        30, 
        0.3, 
        center_of_mass
    );
    
    float timeStep = 1.0f / 600.f;
    int32 velocityIterations = 6;
    int32 positionIterations = 2;

    sf::Clock clock; // starts the clock
    while (main_window->isOpen())
    {
        sf::Event event;
        while (main_window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                main_window->close();
        }

        main_window->clear();
        world.Step(timeStep, velocityIterations, positionIterations);
        my_blob.Draw(main_window);
        main_window->draw(ground);
        my_blob.update();
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}