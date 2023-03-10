#include "Renderer.h"
#include "globals.h"


Renderer::Renderer(sf::RenderWindow& window)
: m_window(window) {}

sf::Vector2f Renderer::worldToScreen(sf::Vector2f worldPosition) {
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

void Renderer::render(Sprite& sprite) {
    // get sprite position in world units
    sf::Vector2f worldPosition = sprite.getPosition();
    // invert y-axis so that positive points upwards
    worldPosition.y *= -1;

    // convert world co-ordinates to screen co-ordinates
    const sf::Vector2f screenPosition = worldToScreen(worldPosition);

    sf::Sprite& spriteRef = sprite.getSFMLSpriteObject();

    // save currentPosition
    auto actualPosition = spriteRef.getPosition();
    // set temporary different position
    spriteRef.setPosition(screenPosition);

    // draw at temporary position
    m_window.draw(spriteRef);

    // reset position back to original
    spriteRef.setPosition(actualPosition);
};