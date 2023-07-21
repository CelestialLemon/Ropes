#pragma once
#include <SFML/Graphics.hpp>

typedef sf::Vector2f vec2;

static float vec2Mag(vec2 vec) {
    return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

static vec2 normalize(vec2 vec) {
    const float mag = vec2Mag(vec);
    return vec2(vec.x / mag, vec.y / mag);
}

static float vec2Dot (vec2 a, vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

static float angleBetweenVec2(vec2 a, vec2 b) {
    return std::atan2f(b.y, b.x) - std::atan2f(a.y, a.x);
}