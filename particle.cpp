#include "particle.h"
#include <SFML/Graphics.hpp>
#include <iostream>

void particle::Draw(std::shared_ptr<sf::RenderWindow> main_window)
{
    m_p = m_p + b2Vec2(.01, .01);
    shape.setPosition(m_p.x, m_p.y);
    main_window->draw(shape);
}