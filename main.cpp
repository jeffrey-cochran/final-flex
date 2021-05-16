#include <SFML/Graphics.hpp>
#include "blob.hpp"
#include "SphereBVH.hpp"
#include "particle.h"
#include "Box2D/Box2D.h"
#include "params.hpp"
#include <iostream>


int main()
{
    //
    // Create world
    b2Vec2 gravity(0.0f, 0.f);
    b2World world(gravity);

    //
    // Set simulation parameters
    float timeStep = 1.0f / 600.f;
    float strain_compliance = 0.;
    
    params::setTimeStep(timeStep);
    params::setStrainCompliance(strain_compliance);

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


    b2Vec2 center_of_mass1(50., 100.);
    b2Vec2 center_of_mass2(150., 100.);
    //
    // Create a bunch of particles
    //rectangle my_blob1(world, sf::Color::Magenta, 40, 40, 0.4, center_of_mass1);
    //rectangle my_blob2(world, sf::Color::Magenta, 40, 40, 0.2, center_of_mass2);
    
    //dogbone dog_blob(world, sf::Color::Magenta, 35, 20, 50, 15, 15, 0.2, center_of_mass1);
    //dog_blob.fixTopShoulder();
    
    bracket b_blob(world, sf::Color::Magenta, 40, 40, .2, center_of_mass1, .45, 1);
     
    //vnotch my_blob1(world, sf::Color::Magenta, 50, 30, 4, 0.5, center_of_mass1);
    //my_blob1.fix(my_blob1.getCenter());
    
    //vnotch my_blob2(world, sf::Color::Magenta, 50, 30, 2, 0.3, center_of_mass2);
    //my_blob2.fix(my_blob2.getCenter());
    
    //segment my_segment(world, sf::Color::Magenta, 15, 30, 0.2, center_of_mass1);
     
    int32 velocityIterations = 6;
    int32 positionIterations = 2;
    
    //my_blob1.fix(0);
    //my_blob1.fix(my_blob1.num_particles()-1);
    
    //my_blob.applyForce(b2Vec2(0, -10000.), 1);
    //my_blob.applyForce(b2Vec2(0, -10000.), 5);

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
        
        world.Step(params::time_step, velocityIterations, positionIterations);

        b_blob.update();
        b_blob.solve_constraints();
        b_blob.Draw(main_window);

        main_window->draw(ground);
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
