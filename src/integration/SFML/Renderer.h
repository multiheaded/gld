//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_RENDERER_H
#define GRIDLOCK_DEFENDERS_SFML_RENDERER_H

#include <SFML/Graphics/RenderTexture.hpp>
#include <Event.h>
#include <iostream>

#include "SFML/Graphics/CircleShape.hpp"

namespace gld {
    namespace integration {
        namespace SFML {
            class Renderer {
            public:
                using RenderTargetType = sf::Texture;
                explicit Renderer(gld::Event::Broker &broker ) : m_broker(broker) {};

                void resize(std::uint32_t width, std::uint32_t height) {
                    [[maybe_unused]] auto result = m_renderTgt.resize({width, height});
                    update_view(static_cast<float>(width), static_cast<float>(height));
                }

                void render() {
                    m_renderTgt.clear();
                    m_renderTgt.setView(m_view);
                    m_renderTgt.display();
                }

                const RenderTargetType& renderTarget() {
                    return m_renderTgt.getTexture();
                }

            private:
                sf::RenderTexture m_renderTgt;
                sf::View m_view;
                [[maybe_unused]] gld::Event::Broker &m_broker;

                void update_view(float width, float height) {
                    m_view.setSize({width, height});
                    m_view.setCenter({0, 0}); // move the coordinate system origin to the center
                    m_renderTgt.setView(m_view);
                }
            };
        } // SFML
    } // integration
} // gld

#endif //GRIDLOCK_DEFENDERS_SFML_RENDERER_H