#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"
#include "Renderer.h"
#include "globals.h"
#include "vector_helper.h"
#include "editor.h"

#include <iostream>
#include <deque>

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
is_editing(true),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}


void Application::run() {

    m_window.setFramerateLimit(120);
    Renderer renderer(m_window);

    sf::Texture pointTexture;
    pointTexture.loadFromFile("./res/images/point_02.png");

    std::vector<PointMass> pms;
    std::vector<Anchor> anchors;

    PointMass* anchor_first_point = nullptr;

    sf::Clock clock;

    vec2 N({0, -1});
    
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                m_window.close();

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.delta == -1)
                    renderer.increaseZoom();
                else if (event.mouseWheelScroll.delta == 1)
                    renderer.decreaseZoom();
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Space) {
                    is_editing = !is_editing;
                }
            }

            if (is_editing) {
                // left button released, place pointmass
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        if (sf::Keyboard::isKeyPressed(sf::Keyboard::LShift)) {
                            editor_place_pm(pms, renderer.getMouseWorldPosition(), pointTexture, PointMassType::STATIC);
                        }
                        else {
                            editor_place_pm(pms, renderer.getMouseWorldPosition(), pointTexture, PointMassType::KINEMATIC);
                        }
                    }
                }

                if (event.type == sf::Event::MouseButtonReleased) {
                    if (event.mouseButton.button == sf::Mouse::Right) {
                        PointMass* closest_pm = find_closest_pointmass(pms, renderer.getMouseWorldPosition());

                        if (anchor_first_point == nullptr) {
                            anchor_first_point = closest_pm;
                            anchor_first_point->sprite.setColor(sf::Color::Blue);
                        }
                        else {

                            if (closest_pm != anchor_first_point) {
                                const float anchor_max_distance = vec2Mag(closest_pm->position - anchor_first_point->position);
                                Anchor new_anchor(anchor_first_point, closest_pm, anchor_max_distance);
                                anchors.push_back(new_anchor);
                                
                            }
                            anchor_first_point->sprite.setColor(sf::Color::White);
                            anchor_first_point = nullptr;
                        }
                    }
                }
            }
        }

        // close app on escape
        // TODO: Remove in final version
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            m_window.close();

        const float dt = clock.restart().asSeconds();
        // const float dt = 0.01;
        // game updates

        if (!is_editing)
        {
            // run simulation
            for (size_t ss = 0; ss < PHYSICS_SUB_STEPS; ss++) {
                for (size_t i = 0; i < pms.size(); i++) {
                    pms[i].AddForce({0, -pms[i].mass * G});
                }
                // addTension(anchors);

                for (size_t i = 0; i < pms.size(); i++) {
                    pms[i].updatePosition(dt / (float)PHYSICS_SUB_STEPS);
                    // pms[i].updateVelocity(dt);
                }
                satisfyContraints(anchors);
            }
        }

        m_window.clear();
        // draw here
        for (size_t i = 0; i < pms.size(); i++) {
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