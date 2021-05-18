#include "params.hpp"
#include "scenario.hpp"
#include "ScenarioData.hpp"


int main(int argc, char* argv[])
{
    
    
    
    //
    // Create world
    b2Vec2 gravity(0.0f, 0.f);
    b2World world(gravity);

    //
    // Set simulation parameters
    char* scenario = argv[1];
    float particles_per_unit_length = atof(argv[2]);
    float timeStep = atof(argv[3]) / 600.f;
    float strain_compliance = atof(argv[4]);
    double f_strain = atof(argv[5]);
    double y_strain = atof(argv[6]);
    double linear_damping = atof(argv[7]);
    int constraint_iters = atoi(argv[8]);
    
    params::setParticlePerUnitLength(particles_per_unit_length);
    params::setTimeStep(timeStep);
    params::setStrainCompliance(strain_compliance);
    params::setFractureStrain(f_strain);
    params::setYieldStrain(y_strain);
    params::setLinearDamping(linear_damping);
    params::setConstraintIters(constraint_iters);
    
    float fixture_friction = 10.f;
    float fixture_density = 10.f;
    params::setParticlePerUnitLength(particles_per_unit_length);
    params::setFixtureFriction(fixture_friction);
    params::setFixtureDensity(fixture_density);
    
    // Create ground box
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
    
    b2Vec2 center;
    
    // Pointer to scenario since the class is abstract
    // Also, char* argv[] has been added, which means you can pass in
    // arguments. Arguments that let us change between Scenarios (although the logic)
    // hasn't been implemented yet
    struct ScenarioData sData = ScenarioData();
    Scenario* test;
    
    if (strcmp(scenario, "lbracket") == 0) {
        center = b2Vec2(120., 100.);
        sData.center_of_mass = center;
        sData.width = 60;
        sData.height = 60;
        sData.thickness = 15;
        
        test = new LBracketBreak(&sData, world);
    } else if (strcmp(scenario, "vnotch") == 0) {
        center = b2Vec2(100., 100.);
        sData.center_of_mass = center;
        sData.width = 80;
        sData.height = 30;
        sData.n_depth = 3;
        
        test = new VNotchBreak(&sData, world);
    } else {
        center = b2Vec2(100., 100.);
        sData.down_strain = -0.001;
        sData.center_of_mass = center;
        sData.shoulder_width = 35;
        sData.shoulder_height = 20;
        sData.neck_height = 45;
        sData.neck_width = 25;
        sData.transition_length = 10;
        test = new DogboneStretch(&sData, world);
    }
    
    
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
