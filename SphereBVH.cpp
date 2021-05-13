#include "SphereBVH.hpp"
#include "box2d/box2d.h"
#include "particle.h"
#include "math.h"

//
// Constructors
SphereNode::SphereNode(){
    this->bounding_volume.radius = 0.;
    this->bounding_volume.center = b2Vec2_zero;
    this->bounding_volume.area = 0.;

    this->left_child = nullptr;
    this->right_child = nullptr;
    this->object = nullptr;

    this->index = -1;
}

SphereNode::SphereNode(std::shared_ptr<particle> p){
    this->bounding_volume.loadFromParticle(p);

    this->index = p->getIndex();
    this->left_child = nullptr;
    this->right_child = nullptr;
    this->object = p;
}

//
// Modules
void SphereNode::write() {
    std::cout << "Node Index: " << this->index << std::endl;
    std::cout << "Center (x,y): " << "(" << this->bounding_volume.center.x << ", " << this->bounding_volume.center.y << ")" << std::endl;
    std::cout << "Radius: " << this->bounding_volume.radius << std::endl;
    std::cout << "Area: " << this->bounding_volume.area << std::endl;
    std::cout << "Has left child: " << (bool)this->left_child << std::endl;
    std::cout << "Has right child: " << (bool)this->right_child << std::endl;
}

void SphereNode::extendBoundingArea(std::shared_ptr<particle> p) {
    if( this->bounding_volume.radius == 0. ) {
        this->bounding_volume.loadFromParticle(p);
    } else if( !(this->contains(p)) ) {
        b2Vec2 delta = p->getPosition()-this->bounding_volume.center;
        this->bounding_volume.radius = 0.5 * (delta.Length() + p->getRadius() + this->bounding_volume.radius);
        this->bounding_volume.center = p->getPosition() - ((this->bounding_volume.radius - p->getRadius()) / delta.Length()) * delta;
        this->bounding_volume.area = M_PI * pow(this->bounding_volume.radius, 2.0);
    }
    return;
}

std::vector<int> SphereNode::intersects(SphereNode& test_node) {
    std::vector<int> intersections;
    //
    // Assume that test_node has no children for now.
    bool is_intersecting = this->intersects_(test_node);

    if( this->index >= 0 && this->index != test_node.index && is_intersecting ) {
        intersections.emplace_back(this->index);
        this->object->setColor(sf::Color::White);
    } else if( this->index < 0 && is_intersecting ) {
        if( this->left_child ) {
            
            std::vector<int> left_intersections = this->left_child->intersects(test_node);
            this->append_indices(intersections, left_intersections);
        }
        if( this->right_child ) {
            std::vector<int> right_intersections = this->right_child->intersects(test_node);
            this->append_indices(intersections, right_intersections);
        }
    }
    return intersections;
}

bool SphereNode::intersects_(SphereNode& test_node) {
    bool intersects = this->contains(test_node) || test_node.contains(*this);
    if( !intersects ) {
        float length = (this->bounding_volume.center - test_node.bounding_volume.center).Length();
        intersects = (length <= this->bounding_volume.radius + test_node.bounding_volume.radius);
    }
    return intersects;
}

void SphereNode::append_indices(std::vector<int>& target, std::vector<int>& source) {
    if( source.size() > 0) {
        target.insert(target.end(), source.begin(), source.end());
    }
}

bool SphereNode::contains(SphereNode& test_node) {
    float length = (this->bounding_volume.center - test_node.bounding_volume.center).Length();
    return (this->bounding_volume.radius - length >= test_node.bounding_volume.radius);
}

bool SphereNode::contains(std::shared_ptr<particle> p) {
    float length = (this->bounding_volume.center - p->getPosition()).Length();
    return (this->bounding_volume.radius - length >= p->getRadius());
}

//
// ==================
// BVH
// ==================
std::shared_ptr<SphereNode> BVH::computeBoundingSphere(std::vector<std::shared_ptr<particle>> particles){
    
    SphereNode new_node;

    for( const auto& p : particles) {
        new_node.extendBoundingArea(p);
    }
    
    return std::make_shared<SphereNode>(new_node);
}

std::shared_ptr<SphereNode> BVH::TopDownSphereBVH(std::vector<std::shared_ptr<particle>> particles) {
    
    std::shared_ptr<SphereNode> p_current_node = computeBoundingSphere(particles);

    std::shared_ptr<SphereNode> left_child;
    std::shared_ptr<SphereNode> right_child;
    
    int size = particles.size();
    int midpoint_index = size / 2;

    if( particles.size() == 2) {
        SphereNode new_left_sphere(particles[0]);
        SphereNode new_right_sphere(particles[1]);

        left_child = std::make_shared<SphereNode>( new_left_sphere );
        right_child = std::make_shared<SphereNode>( new_right_sphere );
        //
    } else if( particles.size() == 1) {
        SphereNode new_left_sphere(particles[0]);

        left_child = std::make_shared<SphereNode>( new_left_sphere );
        right_child = nullptr;
        //
    } else {
        std::vector<std::shared_ptr<particle>> left_particles(&particles[0], &particles[midpoint_index]);
        std::vector<std::shared_ptr<particle>> right_particles(&particles[midpoint_index], &particles[size]);

        left_child = TopDownSphereBVH(left_particles);
        right_child = TopDownSphereBVH(right_particles);
        //
    }


    p_current_node->setChild(left_child, false);
    p_current_node->setChild(right_child, true);

    //
    return p_current_node;
}

