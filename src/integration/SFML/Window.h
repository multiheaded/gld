//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_WINDOW_H
#define GRIDLOCK_DEFENDERS_SFML_WINDOW_H

#include <SFML/Graphics/RenderWindow.hpp>

#include "SFML/Graphics/Sprite.hpp"
#include "../../../include/Event.h"
namespace gld {
    namespace integration {
        namespace SFML {
            class Window {
            public:
                Window(gld::Event::Broker &broker, unsigned int width, unsigned int height, const std::string& title);

                void process_events();

                template<typename RenderTargetT>
                void display(const RenderTargetT &target) {
                    m_window.clear();
                    sf::Sprite tgtSprite(target);
                    m_window.draw(tgtSprite);
                    m_window.display();
                }

            private:
                gld::Event::Broker &m_broker;
                sf::RenderWindow m_window;
            };
        } // SFML
    } // integration
} // gld

#endif //GRIDLOCK_DEFENDERS_SFML_WINDOW_H