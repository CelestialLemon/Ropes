#pragma once

#include "Sprite.h"

#include <deque>
#include <SFML/Graphics.hpp>

typedef sf::Vector2f vec2;

class Renderer {
    private:
    sf::RenderWindow& m_window;
    float zoom;

    // convert world co-ordinates to screen co-ordinates to draw on screen

    // delete default constructor, now allowed
    Renderer() = delete;

    public:
    Renderer(sf::RenderWindow& window);
    void increaseZoom();
    void decreaseZoom();
    void render(Sprite& sprite);
    void render(const sf::VertexArray& va);

    vec2 getMouseWorldPosition() const;
    vec2 worldToScreen(vec2 world_position) const;
    vec2 screenToWorld(vec2 screen_position) const;
};