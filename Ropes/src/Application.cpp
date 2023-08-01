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

const size_t NUM_OF_PMS = 25;

void Application::run() {

    m_window.setFramerateLimit(120);
    Renderer renderer(m_window);

    sf::Texture pointTexture;
    pointTexture.loadFromFile("./res/images/point_03.png");

    std::vector<PointMass> pms(NUM_OF_PMS);

    for (size_t i = 0; i < NUM_OF_PMS; i++) {
        pms[i].sprite.setSpriteTexture(pointTexture);
        pms[i].mass = 1;
        
        if (i == 0) {
            pms[i].pointMassType = PointMassType::STATIC;
        }
        else {
            
            pms[i].pointMassType = PointMassType::KINEMATIC;
        }
        pms[i].position = {0.18f * i, 3.0f};
        pms[i].prevPosition = {0.18f * i, 3.0f};

    }

    std::vector<Anchor> anchors;
    for (size_t i = 0; i < NUM_OF_PMS - 1; i++) {
        Anchor new_anchor(&pms[i], &pms[i + 1], 0.21f);
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
        // const float dt = 0.01;
        // game updates
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            pms[i].AddForce({0, -pms[i].mass * G});
        }

        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            pms[i].updatePosition(dt);
            // pms[i].updateVelocity(dt);
        }
        satisfyContraints(anchors);

        float systemTotalEnergy = 0.0f;
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            systemTotalEnergy += pms[i].getTotalEnergy(dt);
        }
        printf("System Total Energy: %.2f\n", systemTotalEnergy);
        
        m_window.clear();
        // draw here
        for (size_t i = 0; i < NUM_OF_PMS; i++) {
            renderer.render(pms[i].sprite);
        }

        for (const Anchor& anchor : anchors) {
            sf::VertexArray va;
            va.append(sf::Vertex(anchor.anchor_point_A->position));
            va.append(sf::Vertex(anchor.anchor_point_B->position));
            renderer.render(va);
        }

        m_window.display();
    }
}