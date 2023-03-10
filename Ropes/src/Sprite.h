#pragma once

#include <SFML/Graphics.hpp>
#include <string>

// a basic GameObject
// set the params like color position scale etc. All this is stored in the SFML properties for sf::Sprite class

class Sprite {
	// set the m_sprite field with loadTextureFromFile
	sf::Sprite m_sprite;

	// m_texture loads and holds the texture till the object has to be destroyed
	sf::Texture m_texture;

public:
	void loadTextureFromFile(const std::string& filepath);
	// copy loader
	void setSpriteTexture(const sf::Texture& texture);
	
	void setColor(sf::Color color);
	void setPosition(sf::Vector2f pos);
	void setScale(sf::Vector2f scale);
	// set rotation to given deg
	void setRotation(float deg);

	// increase the rotation by given deg
	void rotate(float deg);

	sf::Color getColor() const;
	sf::Vector2f getScale() const;
	sf::Vector2f getPosition() const;
	float getRotation() const;
	sf::FloatRect getGlobalBounds() const;
	sf::Vector2f getOrigin() const;
	sf::Vector2f getDimmensions() const;
	sf::Sprite& getSFMLSpriteObject();
	
	// Move changes the position of the given GameObject with the given Vector2
	void Move(sf::Vector2f dir);
	
	// calls the SFML draw function on the given window and draws m_sprite
	void draw(sf::RenderWindow& window);
};