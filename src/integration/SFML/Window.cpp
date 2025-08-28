//
// Created by marc on 28.08.25.
//

#include "Window.h"

#include <iostream>
#include "../../../include/Event.h"

namespace gld {
    namespace integration {
        namespace SFML {
            Window::Window(gld::Event::Broker &broker, unsigned int width, unsigned int height,
                           const std::string &title)
                : m_broker(broker), m_window(sf::VideoMode({width, height}), title) {
            }

            void Window::process_events() {
                while (const std::optional event = m_window.pollEvent()) {
                    if (event->is<sf::Event::Closed>()) {
                        m_window.close();
                        m_broker.publish<gld::Event::Quit>({});
                    } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                        m_broker.publish<gld::Event::Resized>(
                            {
                                static_cast<std::uint32_t>(resized->size.x),
                                static_cast<std::uint32_t>(resized->size.y)
                            });
                    }
                }
            }
        } // SFML
    } // integration
} // gld
