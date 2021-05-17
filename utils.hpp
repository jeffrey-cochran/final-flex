#ifndef UTILS_HPP
#define UTILS_HPP

#include "box2d/box2d.h"
#include <memory>
#include <map>
#include <set>

namespace utils {
    b2Vec2 phys2vis(b2Vec2 pos);
    extern int current_body_index;
}

struct FixtureUserData {
    std::shared_ptr<
        std::map<std::pair<int, int>, bool>
    > w_flags;
    std::pair<int,int> flag_id;
    FixtureUserData(
        std::shared_ptr<std::map<std::pair<int,int>, bool>> in_w_flags,
        int in_body_index,
        int in_particle_index
    ) {
        w_flags = in_w_flags;
        flag_id = std::pair<int,int>(in_body_index, in_particle_index);
    };
};

#endif
