#include "Physicsbody.h"
#include <SFML/System.hpp>

PhysicsBody::PhysicsBody(float mass, vec2 initVelocity)
: m_mass(mass),
m_velocity(initVelocity) 
{}

void PhysicsBody::AddForce(vec2 force, float dt) {
    vec2 k1 = force / m_mass;                              // Calculate velocity change due to force
    vec2 k2 = (force / m_mass) * 0.5f * dt;                // Same but half applied because we consider this over only half the time in step
    vec2 k3 = (force / m_mass) * 0.5f * dt;                // Same as above again
    vec2 k4 = (force / m_mass) * dt;                       // Calculate at full (dt) time step
    m_velocity += (k1 + (2.0f * (k2 + k3)) + k4) / 6.0f;   // Calculate final velocity change over full time dt based on weighted averages of all changes.
}

void PhysicsBody::ResetVelocity() {
    m_velocity = {0, 0};
}

vec2 PhysicsBody::getVelocity() const {
    return m_velocity;
}

float PhysicsBody::getMass() const {
    return m_mass;
}
