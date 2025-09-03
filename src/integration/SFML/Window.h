//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_WINDOW_H
#define GRIDLOCK_DEFENDERS_SFML_WINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

#include <Event.h>
#include <Window.h>


namespace gld::integration::SFML {
    class Window : public gld::Window<gld::integration::SFML::Window> {
    public:
        Window(gld::EventBroker &broker, unsigned int width, unsigned int height, const std::string &title);

        [[nodiscard]] std::optional<gld::UIEvent> query_event();

        template<typename RenderTargetT>
        void display(const RenderTargetT &target);

    private:
        sf::RenderWindow m_window;
    };
} // gld::integration::SFML

#include "Window.hpp"

#endif //GRIDLOCK_DEFENDERS_SFML_WINDOW_H
