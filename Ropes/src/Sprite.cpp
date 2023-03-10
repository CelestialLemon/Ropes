#include "Sprite.h"

// use SFML in-built function to load the texture and attach it to the m_sprite
void Sprite::loadTextureFromFile(const std::string& filepath) {
    m_texture.loadFromFile(filepath);
    m_sprite.setTexture(m_texture);

    m_sprite.setOrigin(sf::Vector2f((float)m_sprite.getTexture()->getSize().x / 2, (float)m_sprite.getTexture()->getSize().y / 2));
}

// copy texture from another Texture object, set as sprite
void Sprite::setSpriteTexture(const sf::Texture& texture) {
    m_texture = sf::Texture(texture);
    m_sprite.setTexture(m_texture);
}

// move the sprite on the screen, works in pixel co-ordinates
void Sprite::Move(sf::Vector2f dir) {
    auto currentPos = m_sprite.getPosition();
    m_sprite.setPosition(currentPos + dir);
}

// renders m_sprite
void Sprite::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}


// set color for the sprite
// the color set is then multiplied by colors of the image to produce result
void Sprite::setColor(sf::Color color) {
    m_sprite.setColor(color);
}

// set position of the sprite on the screen in pixel co-ordinates
void Sprite::setPosition(sf::Vector2f pos) {
    m_sprite.setPosition(pos);
}

// increase or decrease the size of the sprite
void Sprite::setScale(sf::Vector2f scale) {
    m_sprite.setScale(scale);
}

void Sprite::setRotation(float deg) {
    m_sprite.setRotation(deg);
}

void Sprite::rotate(float deg) {
    m_sprite.rotate(deg);
}

sf::Color Sprite::getColor() { return m_sprite.getColor(); }
sf::Vector2f Sprite::getPosition() { return m_sprite.getPosition(); };
sf::Vector2f Sprite::getScale() {return m_sprite.getScale(); };
float Sprite::getRotation() { return m_sprite.getRotation(); }

// get the bounding rect of the sprite
sf::FloatRect Sprite::getGlobalBounds() { return m_sprite.getGlobalBounds(); }

// get the position of the origin of this sprite
sf::Vector2f Sprite::getOrigin() { return m_sprite.getOrigin(); }

// get the dimmensions of the texture of the sprite
// doesn't factor in scale of the object
sf::Vector2f Sprite::getDimmensions() { 
    return sf::Vector2f(m_sprite.getTexture()->getSize().x, m_sprite.getTexture()->getSize().y); 
}