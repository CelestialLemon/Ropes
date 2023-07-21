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

const size_t NUM_OF_PMS = 5;

void Application::run() {

    m_window.setFramerateLimit(100);
    Renderer renderer(m_window);

    sf::Texture pointTexture;
    pointTexture.loadFromFile("./res/images/point_01.png");

    PointMass* pms = new PointMass[NUM_OF_PMS];

    for (size_t i = 0; i < NUM_OF_PMS; i++) {
        pms[i].sprite.setSpriteTexture(pointTexture);
        pms[i].mass = (NUM_OF_PMS - i) * 10;
        
        if (i == 0) {
            pms[i].pointMassType = PointMassType::STATIC;
        }
        else {
            pms[i].position = {0.75f * i, 0};
            pms[i].prevPosition = {0.75f * i, 0};
            pms[i].pointMassType = PointMassType::KINEMATIC;
        }

    }

    std::vector<Anchor> anchors;
    for (size_t i = 0; i < NUM_OF_PMS - 1; i++) {
        Anchor new_anchor(&pms[i], &pms[i + 1], 0.75f);
        anchors.push_back(new_anchor);
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
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            pms[i].AddForce({0, -pms[i].mass * G}, dt);
        }

        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            pms[i].updatePosition(dt);
            satisfyContraints(anchors);
            pms[i].updateVelocity(dt);
        }

        float systemTotalEnergy = 0.0f;
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            systemTotalEnergy += pms[i].getTotalEnergy();
        }
        printf("System Total Energy: %.2f\n", systemTotalEnergy);
        
        m_window.clear();
        // draw here
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            renderer.render(pms[i].sprite);
        }

        sf::VertexArray va;
        for(size_t i = 0; i < NUM_OF_PMS; i++) {
            va.append(sf::Vertex(pms[i].position, sf::Color::White));
        }

        renderer.render(va);

        m_window.display();
    }
}