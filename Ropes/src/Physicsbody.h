#include <SFML/Graphics.hpp>

typedef sf::Vector2<float> vec2; // Define a new datatype called vec2 which is an alias for sf::Vector2<float>


class PhysicsBody {
    float m_mass;
    sf::Vector2f m_velocity;

    public:
    PhysicsBody(float mass, sf::Vector2f initVelocity);

    void AddForce(vec2 force, float dt);

    // sets velocity back to zero
    void ResetVelocity();

    sf::Vector2f getVelocity() const;
    float getMass() const;
};