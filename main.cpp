#include "params.hpp"
#include "scenario.hpp"


int main(int argc, char* argv[])
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
    groundBox.SetAsBox(500.0f, 13.0f);

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
    
    b2Vec2 center(75., 75.);
    b2Vec2 b_center(100., 130.);
    b2Vec2 b_dis(0.f, -0.001);
    b2Vec2 b_force(0.f, -0.001f);
    
    
    // Pointer to scenario since the class is abstract
    // Also, char* argv[] has been added, which means you can pass in
    // arguments. Arguments that let us change between Scenarios (although the logic)
    // hasn't been implemented yet
    Scenario* test;
    
    // Use this call to test out Dogbone scenario
    // Look at Scenario.cpp for details about the constructor
    //test = new DogboneStretch(45, 25, 25, 15, 10, center, strain, world, 0.2, 1.0, 1.0, 1.0);
    
    test = new VNotchBreak(65, 30, 1, 15, 15, center, b_center, world, b_force, 0.2, 1.0, 1.0, 1.0);
    
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
        
        // Running the scenario
        // Check the run function of the scenario to see how I did this
        test->run(main_window);
        
        main_window->draw(ground);
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
