#pragma once
#include "Sprite.h"
#include "vector_helper.h"
#include "globals.h"

#include <SFML/Graphics.hpp>
#include <unordered_set>

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

    void updatePosition(float dt) {
        if (pointMassType == PointMassType::STATIC) return;

        prevPosition = position;
        position += velocity * dt;
        sprite.setPosition(position);
    }

    void updateVelocity(float dt) {
        velocity = (position - prevPosition) / dt;
    }

    float getTotalEnergy() {
        if (pointMassType == PointMassType::STATIC) return 0;

        const float potential_energy = mass * G * position.y;
        const float kinetic_energy = 0.5f * mass * pow(vec2Mag(velocity), 2);
        return potential_energy + kinetic_energy;
    }
};

struct Anchor {
    PointMass* anchor_point_A;
    PointMass* anchor_point_B;
    float max_length;

    Anchor(PointMass* _anchor_point_A, PointMass* _anchor_point_B, float _max_length) : 
    anchor_point_A(_anchor_point_A),
    anchor_point_B(_anchor_point_B),
    max_length(_max_length)
    {}
};

void satisfyContraints(const std::vector<Anchor>& anchors) {
    for (Anchor anchor : anchors) {
        PointMass* A = anchor.anchor_point_A;
        PointMass* B = anchor.anchor_point_B;

        if ((A->pointMassType == PointMassType::STATIC) &&
            (B->pointMassType == PointMassType::STATIC))
        { return; }
        else 
        {
            vec2 dir = B->position - A->position;
            const float distance = vec2Mag(dir);

            dir = normalize(dir);

            if (distance <= anchor.max_length) return;

            if ((A->pointMassType == PointMassType::STATIC) &&
                (B->pointMassType == PointMassType::KINEMATIC)) 
            {
                B->position += (distance - anchor.max_length) * (-dir);
            }
            else if ((A->pointMassType == PointMassType::KINEMATIC) &&
                    (B->pointMassType == PointMassType::STATIC)) 
            {
                A->position += (distance - anchor.max_length) * dir;
            }
            else 
            {
                const float combinedMass = A->mass + B->mass;
                A->position += (B->mass / combinedMass) * (distance - anchor.max_length) * dir;
                B->position += (A->mass / combinedMass) * (distance - anchor.max_length) * -dir;
            }
        }
        // vec2& otherPosition = anchor.anchor_point->position;
        // const float otherMass = anchor.anchor_point->mass;
        
        // // vector pointing from current pointmass to the anchor pointmass 
        // vec2 dir = otherPosition - position;
        // const float distance = vec2Mag(dir);

        // dir = normalize(dir);

        // if (distance > anchor.max_length) {
        //     if (anchor.anchor_point->pointMassType == PointMassType::STATIC) {
        //         position = position + (distance - anchor.max_length) * dir;
        //     }
        //     else if (anchor.anchor_point->pointMassType == PointMassType::KINEMATIC) {
        //         const float combinedMass = otherMass + mass;
        //         otherPosition += (mass / combinedMass) * (distance - anchor.max_length) * (-dir);
        //         position += (otherMass / combinedMass) * (distance - anchor.max_length) * dir;
        //     } 
        // }
    }
}