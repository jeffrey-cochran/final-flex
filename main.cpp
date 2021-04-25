#include <SFML/Graphics.hpp>
#include "particle.h"
#include <Box2D/Box2D.h>
#include <iostream>

int main()
{
    //
    // Create world
    b2Vec2 gravity(0.0f, 0.1f);
    b2World world(gravity);

    b2BodyDef groundBodyDef;
    groundBodyDef.position.Set(0.0f, 100.f);

    b2Body* groundBody = world.CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    groundBox.SetAsBox(500.0f, 1.0f);

    groundBody->CreateFixture(&groundBox, 0.0f);

    //
    // Create visualizer window
    std::shared_ptr<sf::RenderWindow> main_window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(200, 200), 
        "SFML works!"
    );

    //
    // Create a bunch of particles
    int dims = 3;
    std::vector<particle> particles_(pow(dims,2));
    for (int i = 0;  i < dims; i++){
        for (int j = 0;  j < dims; j++){
            b2Vec2 pos(3*i, 3*j);
            particle p(
                3., 
                dims*i + j, 
                pos, 
                sf::Color::Green,
                world
            );
            particles_.emplace_back(p);
        }
    }
    
    float timeStep = 1.0f / 600.f;
    int32 velocityIterations = 1;
    int32 positionIterations = 1;

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
        for ( auto& s : particles_ ) {
            s.Draw(main_window);
        }
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
