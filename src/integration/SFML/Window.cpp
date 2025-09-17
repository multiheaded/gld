//
// Created by marc on 28.08.25.
//

#include "Window.h"

#include <Event.h>
#include <optional>


namespace gld::integration::SFML {
    Window::Window(gld::EventBroker &broker, unsigned int width, unsigned int height,
                   const std::string &title)
        : gld::Window<gld::integration::SFML::Window>(broker, width, height, title)
          , m_window(sf::VideoMode({width, height}), title) {
        m_window.setMouseCursorVisible(false);
    }

    std::optional<gld::UIEvent> Window::query_event() {
        if (const std::optional event = m_window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                m_window.close();
                return gld::UIEvent(gld::Quit{});
            } else if (const auto *resized = event->getIf<sf::Event::Resized>()) {
                m_window.setView(sf::View(sf::FloatRect({0.0f, 0.0f}, {
                                                            static_cast<float>(resized->size.x),
                                                            static_cast<float>(resized->size.y)
                                                        })));
                return gld::UIEvent(gld::Resized
                    {
                        .size{
                            static_cast<std::uint32_t>(resized->size.x),
                            static_cast<std::uint32_t>(resized->size.y)
                        }
                    });
            } else if (const auto *mousePos = event->getIf<sf::Event::MouseMoved>()) {
                return gld::UIEvent(gld::MouseMoved
                    {
                        gld::Vector2u(
                            static_cast<std::uint32_t>(mousePos->position.x),
                            static_cast<std::uint32_t>(mousePos->position.y)
                        )
                    });
            }
        }
        return std::nullopt;
    }
} // gld::integration::SFML
