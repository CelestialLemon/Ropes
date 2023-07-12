#include "Sprite.h"

#include <deque>
#include <SFML/Graphics.hpp>

class Renderer {
    private:
    sf::RenderWindow& m_window;

    // convert world co-ordinates to screen co-ordinates to draw on screen
    sf::Vector2f worldToScreen(sf::Vector2f world_position);

    // delete default constructor, now allowed
    Renderer() = delete;

    public:
    Renderer(sf::RenderWindow& window);
    void render(Sprite& sprite);
    void render(const sf::VertexArray& va);
};