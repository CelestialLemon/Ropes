#include "Renderer.h"
#include "globals.h"


Renderer::Renderer(sf::RenderWindow& window)
: m_window(window), zoom(1.0f) {}

void Renderer::increaseZoom() {
    zoom *= 1.1;
}

void Renderer::decreaseZoom() {
    zoom /= 1.1;
}

vec2 Renderer::getMouseWorldPosition() const {
    sf::Vector2i mouse_screen_position = sf::Mouse::getPosition(m_window);
    // invert y axis
    return screenToWorld((vec2)mouse_screen_position); 
}

sf::Vector2f Renderer::worldToScreen(sf::Vector2f worldPosition) const {
    // scale by inverse zoom
    worldPosition *= 1.0f / zoom;

    // invert y-axis so that positive points upwards
    worldPosition.y *= -1.0f;

    // world position is in range -0.5W -> 0.5W, -0.5H -> 0.5H
    // convert it to 0 -> W, 0 -> H
    worldPosition += sf::Vector2f(SCREEN_WIDTH_WORLD / 2, SCREEN_HEIGHT_WORLD / 2);

    // convert to range 0 -> 1
    worldPosition.x /= SCREEN_WIDTH_WORLD;
    worldPosition.y /= SCREEN_HEIGHT_WORLD;

    // convert to screen co-ordinates by multiplying with screen dimmensions
    const sf::Vector2f screenPosition(
        worldPosition.x * SCREEN_WIDTH,
        worldPosition.y * SCREEN_HEIGHT
    );

    // return screen position
    return screenPosition;
}

vec2 Renderer::screenToWorld(vec2 screen_position) const {
    // convert screen co-ordinates in the space 0 -> 1
    screen_position.x /= (float)SCREEN_WIDTH;
    screen_position.y /= (float)SCREEN_HEIGHT;

    // convert to range -0.5 to 0.5
    screen_position.x -= 0.5f;
    screen_position.y -= 0.5f;

    // convert to world co-ordinates by multiplying by dimmensions
    return vec2(
        screen_position.x * SCREEN_WIDTH_WORLD, 
        screen_position.y * SCREEN_HEIGHT_WORLD * -1.0f
    );
}

void Renderer::render(Sprite& sprite) {
    // get sprite position in world units
    sf::Vector2f worldPosition = sprite.getPosition();

    // convert world co-ordinates to screen co-ordinates
    const sf::Vector2f screenPosition = worldToScreen(worldPosition);

    sf::Sprite& spriteRef = sprite.getSFMLSpriteObject();

    // save currentPosition
    auto actualPosition = spriteRef.getPosition();
    // set temporary different position
    spriteRef.setPosition(screenPosition);

    // scale the sprite depending on zoom level
    spriteRef.setScale(sf::Vector2f(1.0f / zoom, 1.0f / zoom));

    // draw at temporary position
    m_window.draw(spriteRef);

    // reset position back to original
    spriteRef.setPosition(actualPosition);
};

void Renderer::render(const sf::VertexArray& va) {

    sf::VertexArray val(va);

    for(size_t i = 0; i < val.getVertexCount(); i++) {
        auto& vertex = val[i];
        vertex.position = worldToScreen(vertex.position);
    }

    m_window.draw(&val[0], val.getVertexCount(), sf::LinesStrip);
}