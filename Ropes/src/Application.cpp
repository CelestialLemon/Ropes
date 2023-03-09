#include "Application.h"
#include "Sprite.h"

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

void Application::run() {
    
    Sprite sprite_point;
    sprite_point.loadTextureFromFile("./res/images/point_01.png");
    sprite_point.setPosition(sf::Vector2f(200, 200));

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

        

        m_window.clear();
        
        sprite_point.draw(m_window);
        
        m_window.display();
    }
}