//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_RENDERER_H
#define GRIDLOCK_DEFENDERS_RENDERER_H

#include <memory>

namespace gld {
    template <class RendererIntegrationT>
    class Renderer {
    public:
        using RenderTargetType = RendererIntegrationT::RenderTargetType;
        Renderer() = delete;
        explicit Renderer(gld::Event::Broker &broker) : m_impl(broker), m_broker(broker) {
            broker.subscribe<gld::Event::Resized>([this](const gld::Event::Resized& val) {
                    m_impl.resize(val.width, val.height);
                });
        };

        void render() {
            m_impl.render();
        }

        const RenderTargetType& renderTarget() {
            return m_impl.renderTarget();
        }

    private:
        RendererIntegrationT m_impl;
        gld::Event::Broker &m_broker;
    };
}



#endif //GRIDLOCK_DEFENDERS_RENDERER_H