#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

void Application::run() {
    
    PointMass point(1, PointMassType::KINEMATIC);
    point.setPosition(sf::Vector2f(200, 200));

    sf::Clock clock;
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();
        }

        // close app on escape
        // TODO: Remove in final version
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            m_window.close();

        float dt = clock.restart().asSeconds();
        // run updates 
        point.update(dt);       

        m_window.clear();
        point.draw(m_window);
        m_window.display();
    }
}