#include "PointMass.h"

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