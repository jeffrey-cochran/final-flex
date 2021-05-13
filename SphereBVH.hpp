#ifndef SPHEREBVH_HPP
#define SPHEREBVH_HPP


#include "particle.h"
#include "box2d/box2d.h"

struct BoundingVolume {
    b2Vec2 center;
    float radius;
    float area;
    void loadFromParticle(std::shared_ptr<particle> p);
};

class SphereNode {

public:

    SphereNode();
    SphereNode(std::shared_ptr<particle> p);
    void extendBoundingArea(std::shared_ptr<particle> p);
    void setChild(std::shared_ptr<SphereNode> child, bool is_right);
    std::vector<int> intersects(SphereNode& test_node);
    bool intersects_(SphereNode& test_node);
    bool contains(SphereNode& test_node);
    bool contains(std::shared_ptr<particle> p);
    void write();
    std::shared_ptr<particle> object;
    std::shared_ptr<SphereNode> left_child;
    std::shared_ptr<SphereNode> right_child;
    BoundingVolume bounding_volume;


    int index = -1;
    int cnt=0;

private:



    void append_indices(std::vector<int>& target, std::vector<int>& source);
};

namespace BVH {
    std::shared_ptr<SphereNode> TopDownSphereBVH(
        std::vector<std::shared_ptr<particle>> partition
    );
    std::shared_ptr<SphereNode> computeBoundingSphere(
        std::vector<std::shared_ptr<particle>> particles
    );
}

inline void BoundingVolume::loadFromParticle(std::shared_ptr<particle> p) {
    assert(p);
    this->radius = p->getRadius();
    this->center = p->getPosition();
    this->area = M_PI * pow(this->radius, 2.);
    //
    // void
}

inline void SphereNode::setChild(std::shared_ptr<SphereNode> child, bool is_right) {
    if( is_right ) {
        this->right_child = child;
    } else {
        this->left_child = child;
    }
    //
    // void
}
//
#endif
