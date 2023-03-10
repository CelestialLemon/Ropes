#include "Physicsbody.h"

PhysicsBody::PhysicsBody(float mass, sf::Vector2f initVelocity)
: m_mass(mass),
m_velocity(initVelocity) 
{}

void PhysicsBody::AddForce(sf::Vector2f force) {
    // calculate acceleration produced by force
    const sf::Vector2f acceleration = force / m_mass;
    // add accleration to velocity
    m_velocity += acceleration;
}

sf::Vector2f PhysicsBody::getVelocity() const {
    return m_velocity;
}

float PhysicsBody::getMass() const {
    return m_mass;
}