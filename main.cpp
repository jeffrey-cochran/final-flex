#include <SFML/Graphics.hpp>
#include "particle.h"
#include <Box2D/Box2D.h>
#include <iostream>

int main()
{
    //
    // Create visualizer window
    std::shared_ptr<sf::RenderWindow> main_window = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(200, 200), 
        "SFML works!"
    );

    //
    // Create a bunch of particles
    int dims = 10;
    std::vector<particle> particles_(pow(dims,2));
    for (int i = 0;  i < dims; i++){
        for (int j = 0;  j < dims; j++){
            b2Vec2 pos(i, j);
            particle p(
                2., 
                10*i + j, 
                pos, 
                sf::Color::Green
            );
            particles_.emplace_back(p);
        }
    }
    
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
        for ( auto& s : particles_ ) {
            s.Draw(main_window);
        }
        main_window->display();
    }
    sf::Time elapsed1 = clock.getElapsedTime();
    std::cout << elapsed1.asSeconds() << std::endl;

    return 0;
}
