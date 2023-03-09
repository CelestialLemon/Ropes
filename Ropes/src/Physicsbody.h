#include <SFML/Graphics.hpp>

class PhysicsBody {
    float m_mass;
    sf::Vector2f m_velocity;

    public:
    PhysicsBody(float mass, sf::Vector2f initVelocity);
};