#include "particle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void particle::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    main_window->draw(rendering_shape);
}