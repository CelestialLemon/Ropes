#pragma once

#include "PointMass.h"
#include "Renderer.h"
#include "vector_helper.h"

#include <SFML/Graphics.hpp>
#include <vector>


void editor_place_pm(std::vector<PointMass>& pms, vec2 mouse_position, sf::Texture& texture, PointMassType pointMassType)
{
    PointMass p(1, mouse_position, pointMassType);
    p.sprite.setSpriteTexture(texture);

    pms.push_back(p);
}

PointMass* find_closest_pointmass(std::vector<PointMass>& pms, vec2 mouse_position) {
    // set some arbitrarily large distance
    float closest_distance = 1e12;
    PointMass* closest_pm = nullptr;

    for (size_t i = 0; i < pms.size(); i++) {
        const PointMass& pm = pms[i];

        const float distance = vec2Mag(pm.position - mouse_position);

        if (distance < closest_distance) {
            closest_distance = distance;
            closest_pm = &pms[i];
        }
    }

    return closest_pm;
} 