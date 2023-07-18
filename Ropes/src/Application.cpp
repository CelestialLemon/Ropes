#include "Application.h"
#include "Sprite.h"
#include "PointMass.h"
#include "Renderer.h"
#include "globals.h"

#include <iostream>
#include <deque>

Application::Application(unsigned int resX, unsigned int resY): 
m_resX(resX), 
m_resY(resY),
m_window(sf::VideoMode(resX, resY), "Ropes")
{}

const float LINK_MAX_DISTANCE = 1.0f;

static float vec2Mag(vec2 vec) {
    return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

static vec2 normalize(vec2 vec) {
    const float mag = vec2Mag(vec);
    return vec2(vec.x / mag, vec.y / mag);
}

static float vec2Dot (vec2 a, vec2 b) {
    return (a.x * b.x) + (a.y * b.y);
}

static float angleBetweenVec2(vec2 a, vec2 b) {
    return std::atan2f(b.y, b.x) - std::atan2f(a.y, a.x);
}

void Application::run() {

    m_window.setFramerateLimit(100);
    Renderer renderer(m_window);

    Sprite bob;
    bob.loadTextureFromFile("./res/images/point_01.png");

    Sprite anchor;
    anchor.loadTextureFromFile("./res/images/point_01.png");
    anchor.setColor(sf::Color::Red);

    anchor.setPosition({0, 0});
    bob.setPosition({1, 0});

    bool is_paused = true;

    sf::VertexArray trailPositions;

    const float M = 100;

    PhysicsBody pb(M, {0, 0});

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

        vec2 prevPos = bob.getPosition();
        float dt = clock.restart().asSeconds();
        

        vec2 anchorToBobVec = normalize(bob.getPosition() - anchor.getPosition());    
        const float theta = angleBetweenVec2(N, anchorToBobVec);

        vec2 normalToAnchorToBobVec = theta > 0 ? vec2(anchorToBobVec.y, -anchorToBobVec.x) : vec2(-anchorToBobVec.y, anchorToBobVec.x);

        vec2 force_gravity = vec2(0, -M * G);
        vec2 force_tension = anchorToBobVec * -M * G * cos(theta);

        // vec2 force = normalToAnchorToBobVec * M * G * abs(sin(theta));

        pb.AddForce(force_gravity, dt);
        pb.AddForce(force_tension, dt);

        bob.Move(pb.getVelocity() * dt);

        // link constraint
        float distance = vec2Mag(bob.getPosition() - anchor.getPosition());
        if (distance > LINK_MAX_DISTANCE) {
            bob.setPosition(anchor.getPosition() + normalize(bob.getPosition() - anchor.getPosition()) * LINK_MAX_DISTANCE);
            pb.SetVelocity((bob.getPosition() - prevPos) / dt);
        }

        // trail
        trailPositions.append(sf::Vertex(bob.getPosition(), sf::Color::White));

        m_window.clear();
        // draw here
        renderer.render(bob);
        renderer.render(anchor);
        renderer.render(trailPositions);

        m_window.display();
    }
}