#include "PointMass.h"

#include "globals.h"

PointMass::PointMass()
: m_pointMassType(PointMassType::STATIC),
m_pb(0, {0, 0})
{}

PointMass::PointMass(float mass, PointMassType pointMassType, sf::Vector2f initVelocity)
: m_pointMassType(pointMassType),
m_pb(mass, initVelocity)
{
    loadTextureFromFile("./res/images/point_01.png");
}

void PointMass::AddAnchor(PointMass* anchor_point, const float max_length) {
    Anchor* new_anchor = new Anchor {anchor_point->getPosition(), max_length};
    m_anchors.insert(new_anchor);
}

void PointMass::updatePosition(float dt) {
    // cache current position value
    const sf::Vector2f& currentPosition = getPosition();

    // calculate next position depending on current velocity
    auto nextPosition = currentPosition + m_pb.getVelocity() * dt;

    // loop over anchors to apply force
    for(auto anchor : m_anchors) {
        // calculate distance between anchors point and current point's next position
        auto dir = anchor->anchor_point - nextPosition;
        const float distance = sqrtf(dir.x * dir.x + dir.y * dir.y);
        
        // normalize dir
        dir /= distance;

        // if at next position the point goes outside max length
        // pull towards the anchor point along the string with force equal to gravity
        if(distance > anchor->max_length) {
            m_pb.ResetVelocity();
            m_pb.AddForce(dir * m_pb.getMass() * G, dt);
        }
    }
    // update position directly

    // calculate new next position after adding all the anchor forces
    nextPosition = currentPosition + m_pb.getVelocity() * dt;

    // update position
    setPosition(nextPosition);
}

// must be called very frame for performaing updates such as physics, movements input etc
void PointMass::update(float dt) {

    // don't perform physics for static objects
    if(m_pointMassType == PointMassType::STATIC) return;

    // calculate gravity force to be added every second
    const sf::Vector2f weight(0, m_pb.getMass() * -G);
    // add gravity force scaled by deltaTime
    m_pb.AddForce(weight * dt, dt);

    // update position depending on current velocity
    updatePosition(dt);
}