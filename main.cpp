#include "blob.hpp"
#include "params.hpp"
#include "fixture.hpp"
#include "scenario.hpp"


int main()
{
    //
    // Create world
    b2Vec2 gravity(0.0f, -0.f);
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
    groundBox.SetAsBox(500.0f, 13.0f);
    
    for (b2Vec2 v : groundBox.m_vertices) {
        std::cout << v.x << ", " << v.y << std::endl;
    }

    groundBody->CreateFixture(&groundBox, 0.0f);

    sf::RectangleShape ground; 
    sf::Vector2f pos(0.0f, 197.0f);
    sf::Vector2f size(1000.0, 3.0);
    ground.setSize(size);
    ground.setPosition(pos);
    ground.setOutlineColor(sf::Color::White);

    //
    // Create visualizer window
    std::shared_ptr<sf::RenderWindow> main_window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(200, 200), 
        "Dogbone"
    );
    
    b2Vec2 center(100., 100.);
    b2Vec2 strain(0.f, -0.005);
    
    Scenario* test;
    
    test = new DogboneStretch(45, 25, 25, 15, 10, center, strain, world, 0.2, 1.0, 1.0, 1.0);
    
    
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
        
        world.Step(params::time_step, velocityIterations, positionIterations);
        
        test->run(main_window);
        
        main_window->draw(ground);
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
