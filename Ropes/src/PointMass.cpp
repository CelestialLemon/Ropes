#include "PointMass.h"

#include "globals.h"

PointMass::PointMass()
: m_pointMassType(PointMassType::STATIC),
m_pb(0, sf::Vector2f(0, 0))
{}

PointMass::PointMass(float mass, PointMassType pointMassType, sf::Vector2f initVelocity)
: m_pointMassType(pointMassType),
m_pb(mass, initVelocity)
{
    loadTextureFromFile("./res/images/point_01.png");
}

void PointMass::updatePosition(float dt) {
    // get current position of the sprite
    sf::Vector2f currentPosition = getPosition();

    // calculate and add displacement
    currentPosition += m_pb.getVelocity() * dt;

    // set new position
    setPosition(currentPosition);
}

// must be called very frame for performaing updates such as physics, movements input etc
void PointMass::update(float dt) {
    // calculate gravity force to be added every second
    const sf::Vector2f weight(0, m_pb.getMass() * -G);
    // add gravity force scaled by deltaTime
    m_pb.AddForce(weight * dt);

    // update position depending on current velocity
    updatePosition(dt);
}