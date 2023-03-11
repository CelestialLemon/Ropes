#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"
#include "Renderer.h"

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

void Application::run() {
    
    Renderer renderer(m_window);

    const float STRING_LENGTH = 2.0f;

    PointMass point1(1, PointMassType::STATIC);
    point1.setPosition(sf::Vector2f(0, 2.0));

    PointMass point2(10, PointMassType::KINEMATIC);
    point2.setPosition(sf::Vector2f(0.0, 2.0));

    point2.AddAnchor(&point1, STRING_LENGTH);

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
        point1.update(dt);
        point2.update(dt);       

        m_window.clear();
        renderer.render(point1);
        renderer.render(point2);
        m_window.display();
    }
}