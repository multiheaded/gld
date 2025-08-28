//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_RENDERER_H
#define GRIDLOCK_DEFENDERS_SFML_RENDERER_H

#include <SFML/Graphics/RenderTexture.hpp>
#include <Event.h>

namespace gld {
    namespace integration {
        namespace SFML {
            class Renderer {
            public:
                using RenderTargetType = sf::Texture;
                explicit Renderer(gld::Event::Broker &broker ) : m_broker(broker) {
                    broker.subscribe<gld::Event::Resized>([this](const gld::Event::Resized& val) -> bool {
                        bool result = m_renderTgt.resize({val.width, val.height});
                        return result;
                    });
                };

                const RenderTargetType& renderTarget() {
                    return m_renderTgt.getTexture();
                }

            private:
                sf::RenderTexture m_renderTgt;
                gld::Event::Broker &m_broker;
            };
        } // SFML
    } // integration
} // gld

#endif //GRIDLOCK_DEFENDERS_SFML_RENDERER_H