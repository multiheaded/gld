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
        explicit Renderer(gld::Event::Broker &broker) : m_impl(broker) {};

        const RenderTargetType& renderTarget() {
            return m_impl.renderTarget();
        }

    private:
        RendererIntegrationT m_impl;
    };
}



#endif //GRIDLOCK_DEFENDERS_RENDERER_H