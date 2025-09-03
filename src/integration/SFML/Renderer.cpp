//
// Created by marc on 28.08.25.
//

#include "Renderer.h"

#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace gld::integration::SFML {

    Renderer::Renderer(gld::EventBroker &broker)
        : gld::Renderer<gld::integration::SFML::Renderer>(broker) {
    };

    void Renderer::resize(std::uint32_t width, std::uint32_t height) {
        [[maybe_unused]] auto result = m_renderTgt.resize({width, height});
        update_view(static_cast<float>(width), static_cast<float>(height));
    }

    void Renderer::clear() {
        m_renderTgt.clear();
        m_renderTgt.setView(m_view);
    }

    void Renderer::finish() {
        m_renderTgt.display();
    }

    void Renderer::draw(const gld::Sprite &s, const Eigen::Vector2f &p) {
        const sf::Texture &texture = m_textures[s.id];
        sf::Vector2f origin {texture.getSize()};
        sf::Sprite sprite(texture);
        sprite.setPosition({p[0],p[1]});
        sprite.setOrigin(origin*0.5f);
        m_renderTgt.draw(sprite);
    }

    Eigen::Vector2i Renderer::un_project(Eigen::Vector2f p) {
        sf::Vector2i r = m_renderTgt.mapCoordsToPixel({p(0), p(1)});
        return Eigen::Vector2i(r.x, r.y);
    }

    Eigen::Vector2f Renderer::project(Eigen::Vector2i p) {
        sf::Vector2f r = m_renderTgt.mapPixelToCoords({p[0], p[1]});
        return Eigen::Vector2f(r.x, r.y);
    }


    gld::Sprite Renderer::make_sprite(const std::string &filename) {
        m_textures.emplace_back(sf::Texture(filename));
        return {.id = m_textures.size() - 1};
    }

    const sf::Texture &Renderer::renderTarget() const {
        return m_renderTgt.getTexture();
    }

    void Renderer::update_view(float width, float height) {
        m_view.setSize({width, height});
        m_view.setCenter({0, 0}); // move the coordinate system origin to the center
        m_renderTgt.setView(m_view);
    }
} // gld::integration::SFML
