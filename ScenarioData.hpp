#ifndef SCENARIODATA_HPP
#define SCENARIODATA_HPP

#include "box2d/box2d.h"

struct ScenarioData {
    int width;
    int height;
    int thickness;
    int n_depth;
    float orientation = 45.0;
    int shoulder_width;
    int shoulder_height;
    int neck_height;
    int neck_width;
    int transition_length;
    float down_strain = -0.001;
    b2Vec2 center_of_mass;
    int b_w = 15;
    int b_h = 15;
    b2Vec2 b1_c = b2Vec2(117., 140.);
    b2Vec2 b2_c = b2Vec2(65., 140.);
    b2Vec2 b3_c = b2Vec2(45., 45.);
    b2Vec2 b4_c = b2Vec2(137., 45.);
};

#endif
