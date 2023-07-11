#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"
#include "Renderer.h"

#include <iostream>

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

void Application::run() {

    m_window.setFramerateLimit(100);
    Renderer renderer(m_window);

    const float K = 2500; // N/m
    const float A = 2.5f;
    const float M = 100;

    sf::Vector2f Fr = {0, 0};
    
    Sprite point;
    point.loadTextureFromFile("./res/images/point_01.png");
    point.setPosition({A, 0});

    PhysicsBody pb(M, {0, 0});

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
        const float X = point.getPosition().x;

        Fr = {- K * X, 0};

        pb.AddForce(Fr, dt);

        point.setPosition(point.getPosition() + pb.getVelocity() * dt);

        m_window.clear();
        // draw here
        renderer.render(point);

        m_window.display();
    }
}