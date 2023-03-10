#include <SFML/Graphics.hpp>
#include "Sprite.h"
#include "Physicsbody.h"

enum class PointMassType {
    STATIC = 0,
    KINEMATIC = 1
};

class PointMass : public Sprite {
    private:
    PhysicsBody m_pb;
    PointMassType m_pointMassType;

    void updatePosition(float dt);

    // default constructor not allowed
    PointMass();
    public:

    // point mass constructor, default initial velocity is 0
    PointMass(float mass, PointMassType pointMassType, sf::Vector2f initVelocity = sf::Vector2f(0, 0));

    void update(float dt);
};