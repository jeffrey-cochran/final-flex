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
    float strain_compliance = 0.1;
    double f_strain = 0.1;
    double y_strain = 0.1;
    
    float fixture_friction = 10.f;
    float fixture_density = 10.f;
    params::setFixtureFriction(fixture_friction);
    params::setFixtureDensity(fixture_density);
    
    params::setTimeStep(timeStep);
    params::setStrainCompliance(strain_compliance);
    params::setFractureStrain(f_strain);
    params::setYieldStrain(y_strain);

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
    
    b2Vec2 center(120., 100.);
    b2Vec2 b1_center(117., 140.);
    b2Vec2 b2_center(65., 140);
    b2Vec2 b3_center(45., 45);
    b2Vec2 b4_center(137., 45);
    
    b2Vec2 b1_dis(0.f, -0.001);
    b2Vec2 b2_dis(0.f, -0.001);
    b2Vec2 b3_dis(0.f, 0.001);
    b2Vec2 b4_dis(0.f, 0.001);
    
    int b_w = 15;
    int b_h = 15;
    
    
    // Pointer to scenario since the class is abstract
    // Also, char* argv[] has been added, which means you can pass in
    // arguments. Arguments that let us change between Scenarios (although the logic)
    // hasn't been implemented yet
    Scenario* test;
    
    // Use this call to test out Dogbone scenario
    // Look at Scenario.cpp for details about the constructor
    //test = new DogboneStretch(45, 25, 25, 15, 10, center, strain, world, 0.2, 1.0, 1.0, 1.0);
    
    //test = new VNotchBreak(75, 30, 6, center, b_dis, world, 0.6, 1.0, 1.0, 1.0);
    
    bracket LBracket(world, sf::Color::White, 60, 60, 15, 0.4, center, 0.0, 0.0);
    fixture b1(0, b_w, b_h, 45.0, b1_center, sf::Color::Green, world);
    fixture b2(1, b_w, b_h, 45.0, b2_center, sf::Color::Green, world);
    fixture b3(2, b_w, b_h, 45.0, b3_center, sf::Color::Green, world);
    fixture b4(3, b_w, b_h, 45.0, b4_center, sf::Color::Green, world);
    
    b1.addDisplacement(b1_dis);
    b2.addDisplacement(b2_dis);
    b3.addDisplacement(b3_dis);
    b4.addDisplacement(b4_dis);
    
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
        
        //test->run(main_window);
        LBracket.update();
        LBracket.solve_constraints();
        LBracket.Draw(main_window);
        
        b1.update();
        b1.applyDisplacement();
        b1.Draw(main_window);
        
        b2.update();
        b2.applyDisplacement();
        b2.Draw(main_window);
        
        b3.update();
        b3.applyDisplacement();
        b3.Draw(main_window);
        
        b4.update();
        b4.applyDisplacement();
        b4.Draw(main_window);
    
        
        main_window->draw(ground);
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
