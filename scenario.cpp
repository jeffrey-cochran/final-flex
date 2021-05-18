#include "scenario.hpp"
#include "fixture.hpp"


Scenario::Scenario() {
    
}

DogboneStretch::DogboneStretch(struct ScenarioData* data, b2World& world)
: bone_(world, sf::Color::White, data->shoulder_width, data->shoulder_height, data->neck_height, data->neck_width,
      data->transition_length, data->center_of_mass)
{
    
    bone_.fixTopShoulder();
    bone_.applyBottomStrain(b2Vec2(0.f, data->down_strain));
}

void DogboneStretch::run(std::shared_ptr<sf::RenderWindow> window) {
    bone_.update();
    bone_.solve_constraints();
    bone_.Draw(window);
}

VNotchBreak::VNotchBreak(struct ScenarioData* data, b2World& world)
: vn_(world, sf::Color::White, data->width, data->height, data->n_depth, data->center_of_mass)
{
    vn_.fixLeftTopEdge();
    vn_.fixLeftBottomEdge();
    vn_.applyRightStrain(b2Vec2(0.f, data->down_strain));
}

void VNotchBreak::run(std::shared_ptr<sf::RenderWindow> window) {
    vn_.update();
    vn_.solve_constraints();
    vn_.Draw(window);
};

LBracketBreak::LBracketBreak(struct ScenarioData* data, b2World& world)
: lbracket_(world, sf::Color::White, data->width, data->height, data->thickness, data->center_of_mass),
b1_(0, data->b_w, data->b_h, data->orientation, data->b1_c, sf::Color::Green, world),
b2_(1, data->b_w, data->b_h, data->orientation, data->b2_c,  sf::Color::Green, world),
b3_(2, data->b_w, data->b_h, data->orientation, data->b3_c, sf::Color::Green, world),
b4_(3, data->b_w, data->b_h, data->orientation, data->b4_c, sf::Color::Green, world)
{
    b1_.addDisplacement(b2Vec2(0.f, data->down_strain));
    b2_.addDisplacement(b2Vec2(0.f, data->down_strain));
    b3_.addDisplacement(b2Vec2(0.f, -data->down_strain));
    b4_.addDisplacement(b2Vec2(0.f, -data->down_strain));
}

void LBracketBreak::run(std::shared_ptr<sf::RenderWindow> window) {
    lbracket_.update();
    lbracket_.solve_constraints();
    lbracket_.Draw(window);
    
    b1_.update();
    b1_.applyDisplacement();
    b1_.Draw(window);
    
    b2_.update();
    b2_.applyDisplacement();
    b2_.Draw(window);
    
    b3_.update();
    b3_.applyDisplacement();
    b3_.Draw(window);
    
    b4_.update();
    b4_.applyDisplacement();
    b4_.Draw(window);
}

