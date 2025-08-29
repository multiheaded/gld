//
// Created by marc on 28.08.25.
//

#include "Window.h"

#include <Event.h>


namespace gld::integration::SFML {
    Window::Window(gld::Event::Broker &broker, unsigned int width, unsigned int height,
                   const std::string &title)
        : gld::Window<gld::integration::SFML::Window>(broker, width, height, title)
        , m_window(sf::VideoMode({width, height}), title) {
    }

    void Window::process_events() {
        while (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
                m_broker.publish<gld::Event::Quit>({});
            } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                m_window.setView(sf::View(sf::FloatRect({0.0f, 0.0f}, {
                                                            static_cast<float>(resized->size.x),
                                                            static_cast<float>(resized->size.y)
                                                        })));
                m_broker.publish<gld::Event::Resized>(
                    {
                        static_cast<std::uint32_t>(resized->size.x),
                        static_cast<std::uint32_t>(resized->size.y)
                    });
            } else if (const auto *mousePos = event->getIf<sf::Event::MouseMoved>()) {
                m_broker.publish<gld::Event::MouseMoved>(
                    {
                        static_cast<std::uint32_t>(mousePos->position.x),
                        static_cast<std::uint32_t>(mousePos->position.y)
                    }
                );
            }
        }
    }
} // gld::integration::SFML
