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

    sf::Texture pointTexture;
    pointTexture.loadFromFile("./res/images/point_01.png");

    PointMass* pms = new PointMass[5];

    for (size_t i = 0; i < 5; i++) {
        pms[i].sprite.setSpriteTexture(pointTexture);
        pms[i].mass = 1;
        
        if (i == 0) {
            pms[i].pointMassType = PointMassType::STATIC;
        }
        else {
            pms[i].position = {0.75f * i, 0};
            pms[i].prevPosition = {0.75f * i, 0};
            pms[i].pointMassType = PointMassType::KINEMATIC;
        }

    }

    for(size_t i = 1; i < 5; i++) {
        pms[i].AddAnchor(new Anchor(&pms[i - 1], 0.75f));
        // pms[i].AddAnchor(new Anchor(&pms[i + 1], 0.75f));
    }

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
        for (size_t i = 0; i < 5; i++) {
            pms[i].AddForce({0, -pms[i].mass * G}, dt);
        }

        for (size_t i = 0; i < 5; i++) {
            pms[i].updatePosition(dt);
        }

        m_window.clear();
        // draw here
        for (size_t i = 0; i < 5; i++) {
            renderer.render(pms[i].sprite);
        }

        m_window.display();
    }
}