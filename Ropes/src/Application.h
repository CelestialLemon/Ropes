#include <SFML/Graphics.hpp>

class Application {
    public:
    Application(unsigned int resX, unsigned int resY);
    void run();

    private:
    unsigned int m_resX;
    unsigned int m_resY;

    sf::RenderWindow m_window;
};