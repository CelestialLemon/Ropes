#include "Sprite.h"
#include "Physicsbody.h"

#include <SFML/Graphics.hpp>
#include <unordered_set>

struct Anchor {
    sf::Vector2f anchor_point;
    float max_length;
};

enum class PointMassType {
    STATIC = 0,
    KINEMATIC = 1
};

class PointMass : public Sprite {
    private:
    PhysicsBody m_pb;
    PointMassType m_pointMassType;

    // hold references of all the points this point is anchored to
    std::unordered_set<Anchor*> m_anchors;

    void updatePosition(float dt);

    // default constructor not allowed
    PointMass();
    public:

    // point mass constructor, default initial velocity is 0
    PointMass(float mass, PointMassType pointMassType, sf::Vector2f initVelocity = sf::Vector2f(0, 0));

    // attach this point mass to another point mass, so that they both can't go more than string length away
    void AddAnchor(PointMass* anchor_point, const float string_length);

    void update(float dt);
};