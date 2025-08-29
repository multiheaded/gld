//
// Created by marc on 28.08.25.
//

#include "Renderer.h"

namespace gld::integration::SFML {

    Renderer::Renderer(gld::Event::Broker &broker)
        : gld::Renderer<gld::integration::SFML::Renderer>(broker) {
    };

    void Renderer::resize(std::uint32_t width, std::uint32_t height) {
        [[maybe_unused]] auto result = m_renderTgt.resize({width, height});
        update_view(static_cast<float>(width), static_cast<float>(height));
    }

    void Renderer::render() {
        m_renderTgt.clear();
        m_renderTgt.setView(m_view);
        m_renderTgt.display();
    }

    const sf::Texture &Renderer::renderTarget() {
        return m_renderTgt.getTexture();
    }

    void Renderer::update_view(float width, float height) {
        m_view.setSize({width, height});
        m_view.setCenter({0, 0}); // move the coordinate system origin to the center
        m_renderTgt.setView(m_view);
    }
} // gld::integration::SFML
