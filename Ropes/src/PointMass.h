#pragma once
#include "Sprite.h"
#include "vector_helper.h"

#include <SFML/Graphics.hpp>
#include <unordered_set>

struct PointMass;

struct Anchor {
    PointMass* anchor_point;
    float max_length;

    Anchor(PointMass* _anchor_point, float _max_length) 
    : anchor_point(_anchor_point),
    max_length(_max_length)
    {}
};

enum class PointMassType {
    STATIC = 0,
    KINEMATIC = 1
};

struct PointMass {
    Sprite sprite;
    float mass;
    vec2 position;
    vec2 prevPosition;
    vec2 velocity;
    std::unordered_set<Anchor*> anchors;
    PointMassType pointMassType;

    PointMass():
    mass(1),
    position({0, 0}),
    prevPosition({0, 0}),
    velocity ({0, 0}),
    pointMassType(PointMassType::KINEMATIC) {} 

    PointMass(float _mass, vec2 _position, PointMassType _pointMassType = PointMassType::KINEMATIC): 
    mass(_mass), 
    position(_position),
    prevPosition(_position),
    pointMassType(_pointMassType),
    sprite(Sprite()) {}

    void AddForce(vec2 force, float dt) {
        vec2 acc = (force / mass);
        velocity += acc * dt;
    }


    void AddAnchor(Anchor* new_anchor) {
        anchors.insert(new_anchor);
    }

    void updatePosition(float dt) {
        if (pointMassType == PointMassType::STATIC) return;

        prevPosition = position;
        position += velocity * dt;
        satisfyContraints();
        sprite.setPosition(position);
        velocity = (position - prevPosition) / dt;
    }

    void satisfyContraints() {
        for (Anchor* anchor : anchors) {
            vec2& otherPosition = anchor->anchor_point->position;
            const float otherMass = anchor->anchor_point->mass;
            
            // vector pointing from current pointmass to the anchor pointmass 
            vec2 dir = otherPosition - position;
            const float distance = vec2Mag(dir);

            dir = normalize(dir);

            if (distance > anchor->max_length) {
                if (anchor->anchor_point->pointMassType == PointMassType::STATIC) {
                    position = position + (distance - anchor->max_length) * dir;
                }
                else if (anchor->anchor_point->pointMassType == PointMassType::KINEMATIC) {
                    const float combinedMass = otherMass + mass;
                    otherPosition += (mass / combinedMass) * (distance - anchor->max_length) * (-dir);
                    position += (otherMass / combinedMass) * (distance - anchor->max_length) * dir;
                } 
            }
        }
    }
};