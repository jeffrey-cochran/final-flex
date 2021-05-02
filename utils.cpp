#include "Box2D/Box2D.h"
#include "utils.hpp"


b2Vec2 utils::phys2vis(b2Vec2 phys_pos) {
    b2Vec2 vis_pos(phys_pos.x, 200 - phys_pos.y);
    return vis_pos;
}