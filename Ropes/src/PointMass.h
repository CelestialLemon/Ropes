#pragma once
#include "Sprite.h"
#include "vector_helper.h"
#include "globals.h"

#include <SFML/Graphics.hpp>
#include <unordered_set>
#include <unordered_map>
#include <iostream>

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
    vec2 force;
    PointMassType pointMassType;

    PointMass():
    mass(1),
    position({0, 0}),
    prevPosition({0, 0}),
    velocity ({0, 0}),
    pointMassType(PointMassType::KINEMATIC),
    sprite(Sprite()),
    force({0, 0}) {} 

    PointMass(float _mass, vec2 _position, PointMassType _pointMassType = PointMassType::KINEMATIC): 
    mass(_mass), 
    position(_position),
    prevPosition(_position),
    pointMassType(_pointMassType),
    sprite(Sprite()),
    velocity({0, 0}),
    force({0, 0}) {}

    void AddForce(vec2 _force) {
        force += _force;
    }

    void updatePosition(float dt) {
        if (pointMassType == PointMassType::KINEMATIC) {
            velocity = (position - prevPosition);
            prevPosition = position;
            position = position + velocity + (force / mass) * dt * dt;
            force = {0, 0};
        }
        sprite.setPosition(position);
    }

    float getTotalEnergy(float dt) {
        if (pointMassType == PointMassType::STATIC) return 0;

        const float potential_energy = mass * G * position.y;
        const float kinetic_energy = 0.5f * mass * pow(vec2Mag((position - prevPosition) / dt), 2);
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

void addTension(std::vector<Anchor>& anchors) {
    for (Anchor& anchor: anchors) {
       PointMass* A = anchor.anchor_point_A;
        PointMass* B = anchor.anchor_point_B;

        if ((A->pointMassType == PointMassType::STATIC) &&
            (B->pointMassType == PointMassType::STATIC))
        { continue; }
        else 
        {
            vec2 dir = B->position - A->position;
            const float distance = vec2Mag(dir);

            dir = normalize(dir);

            if (distance >= anchor.max_length) {
                // apply tension
                const vec2 N = {0, -1};

                const float thetaA = angleBetweenVec2(N, dir);
                const float thetaB = (2 * PI) - thetaA;

                const vec2 Ta = dir * (-A->mass * G * cosf(thetaA));
                const vec2 Tb = -dir * (-B->mass * G * cosf(thetaB));

                A->AddForce(Ta);
                B->AddForce(Tb);
            }
        } 
    }
}

void satisfyContraints(std::vector<Anchor>& anchors) {

    std::unordered_map<PointMass*, std::vector<vec2>> satisfied_positions;
    for (Anchor& anchor : anchors) {
        PointMass* A = anchor.anchor_point_A;
        PointMass* B = anchor.anchor_point_B;

        if ((A->pointMassType == PointMassType::STATIC) &&
            (B->pointMassType == PointMassType::STATIC))
        { continue; }
        else 
        {
            vec2 dir = B->position - A->position;
            const float distance = vec2Mag(dir);

            dir = normalize(dir);

            if (distance <= anchor.max_length) continue;

            if ((A->pointMassType == PointMassType::STATIC) &&
                (B->pointMassType == PointMassType::KINEMATIC)) 
            {
                const vec2 satisfied_position = B->position + (distance - anchor.max_length) * (-dir);

                if (satisfied_positions.find(B) != satisfied_positions.end()) {
                    // pointmass already exists
                    satisfied_positions[B].push_back(satisfied_position);
                }
                else {
                    std::vector<vec2> positions{satisfied_position};
                    satisfied_positions[B] = positions;
                }
            }
            else if ((A->pointMassType == PointMassType::KINEMATIC) &&
                    (B->pointMassType == PointMassType::STATIC)) 
            {
                const vec2 satisfied_position = A->position + (distance - anchor.max_length) * dir;
                
                if (satisfied_positions.find(A) != satisfied_positions.end()) {
                    // pointmass already exists
                    satisfied_positions[A].push_back(satisfied_position);
                }
                else {
                    std::vector<vec2> positions{satisfied_position};
                    satisfied_positions[A] = positions;
                }
            }
            else 
            {
                const float combinedMass = A->mass + B->mass;
                const vec2 satisfied_position_A = A->position + (B->mass / combinedMass) * (distance - anchor.max_length) * dir;
                const vec2 satisfied_position_B = B->position + (A->mass / combinedMass) * (distance - anchor.max_length) * -dir;

                if (satisfied_positions.find(A) != satisfied_positions.end()) {
                    // pointmass already exists
                    satisfied_positions[A].push_back(satisfied_position_A);
                }
                else {
                    std::vector<vec2> positions{satisfied_position_A};
                    satisfied_positions[A] = positions;
                }

                if (satisfied_positions.find(B) != satisfied_positions.end()) {
                    // pointmass already exists
                    satisfied_positions[B].push_back(satisfied_position_B);
                }
                else {
                    std::vector<vec2> positions{satisfied_position_B};
                    satisfied_positions[B] = positions;
                }
            }
        }
    }

    // std::cout << "size of map: " << satisfied_positions.size() << std::endl;
    for(auto& satisfied_position: satisfied_positions) {
        vec2 average_position = {0, 0};

        for(const vec2& position: satisfied_position.second) {
            average_position += position;
        }

        average_position /= (float)satisfied_position.second.size();
        satisfied_position.first->position = average_position;
    }
}