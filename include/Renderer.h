//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_RENDERER_H
#define GRIDLOCK_DEFENDERS_RENDERER_H

#include <memory>
#include <Event.h>

namespace gld {
    template <class RendererIntegrationT>
    class Renderer {
    public:
        Renderer() = delete;
        explicit Renderer(gld::Event::Broker &broker) : m_broker(broker) {
            broker.subscribe<gld::Event::Resized>([this](const gld::Event::Resized& val) {
                    derived().resize(val.width, val.height);
                });
        };

        void render() {
            derived().render();
        }

        const auto& renderTarget() {
            return derived().renderTarget();
        }

        constexpr RendererIntegrationT &derived() {
            return static_cast<RendererIntegrationT &>(*this);
        }

    protected:
        gld::Event::Broker &m_broker;
    };
}



#endif //GRIDLOCK_DEFENDERS_RENDERER_H