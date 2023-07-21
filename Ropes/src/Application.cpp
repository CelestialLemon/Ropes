#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"
#include "Renderer.h"
#include "globals.h"
#include "vector_helper.h"

#include <iostream>
#include <deque>

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

const float LINK_MAX_DISTANCE = 1.0f;

void Application::run() {

    m_window.setFramerateLimit(100);
    Renderer renderer(m_window);

    PointMass pm1(1, {0, 0}, PointMassType::STATIC);
    PointMass pm2(1, {1, 0}, PointMassType::KINEMATIC);
    PointMass pm3(1, {2, 0}, PointMassType::KINEMATIC);

    sf::Texture pointTexture;
    pointTexture.loadFromFile("./res/images/point_01.png");

    pm1.sprite.setSpriteTexture(pointTexture);
    pm2.sprite.setSpriteTexture(pointTexture);
    pm3.sprite.setSpriteTexture(pointTexture);

    pm2.AddAnchor(new Anchor(&pm1, 1.5f));
    // pm2.AddAnchor(new Anchor(&pm1, 1.0f));

    pm3.AddAnchor(new Anchor(&pm2, 1.5f));
    // pm3.AddAnchor(new Anchor(&pm2, 1.0f));

    sf::Clock clock;

    vec2 N({0, -1});
    
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

        const float dt = clock.restart().asSeconds();

        // game updates
        pm1.AddForce({0, - pm1.mass * G}, dt);
        pm2.AddForce({0, - pm2.mass * G}, dt);
        pm3.AddForce({0, - pm3.mass * G}, dt);

        pm1.updatePosition(dt);
        pm2.updatePosition(dt);
        pm3.updatePosition(dt);

        m_window.clear();
        // draw here
        renderer.render(pm1.sprite);
        renderer.render(pm2.sprite);
        renderer.render(pm3.sprite);

        m_window.display();
    }
}