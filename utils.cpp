#include "box2d/box2d.h"
#include "utils.hpp"

namespace utils {
    b2Vec2 phys2vis(b2Vec2 phys_pos) {
        b2Vec2 vis_pos(phys_pos.x, 200 - phys_pos.y);
        return vis_pos;
    }

    int current_body_index = 0;

}

bool b2ContactFilter::ShouldCollide( b2Fixture* fixtureA, b2Fixture* fixtureB ) {
	FixtureUserData* fix_a_data = (FixtureUserData*) fixtureA->GetUserData().pointer;
	FixtureUserData* fix_b_data = (FixtureUserData*) fixtureB->GetUserData().pointer;

	return (
		fix_a_data->w_flags->count(fix_b_data->flag_id) == 0
	) || (
		fix_b_data->w_flags->count(fix_a_data->flag_id) == 0
	);
}
