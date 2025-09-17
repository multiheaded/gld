//
// Created by marc on 28.08.25.
//

#include "Renderer.h"
#include <filesystem>

#include "SFML/Graphics/CircleShape.hpp"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/Sprite.hpp"

namespace gld::integration::SFML {
    Renderer::Renderer(gld::EventBroker &broker)
        : gld::Renderer<gld::integration::SFML::Renderer>(broker) {
    };

    void Renderer::resize(const gld::Vector2u &to_size) {
        [[maybe_unused]] auto result = m_renderTgt.resize({static_cast<std::uint32_t>(to_size.x()), static_cast<std::uint32_t>(to_size.y())});
        update_view(to_size.cast<float>());
    }

    void Renderer::zoom(const gld::Vector2f &to_size) {
        m_zoomTo = to_size;
    }

    void Renderer::clear() {
        m_renderTgt.clear();
        m_renderTgt.setView(m_view);
    }

    void Renderer::finish() {
        m_renderTgt.display();
    }

    void Renderer::draw(const gld::Sprite &s, const gld::Vector2f &p) {
        const sf::Texture &texture = m_textures[s.id];
        sf::Vector2i pos{static_cast<int>(s.rect.position.x()), static_cast<int>(s.rect.position.y())};
        sf::Vector2i size{static_cast<int>(s.rect.size.x()), static_cast<int>(s.rect.size.y())};
        sf::Vector2f origin{static_cast<float>(s.center.x()), static_cast<float>(s.center.y())};
        sf::Sprite sprite(texture, sf::IntRect{pos, size});
        sprite.setPosition({p.x(),p.y()});
        sprite.setOrigin(origin);
        m_renderTgt.draw(sprite);
    }

    gld::Vector2u Renderer::un_project(const gld::Vector2f &p) const {
        sf::Vector2i r = m_renderTgt.mapCoordsToPixel({p.x(), p.y()});
        return {r.x, r.y};
    }

    gld::Vector2f Renderer::project(const gld::Vector2u &p) const {
        sf::Vector2f r = m_renderTgt.mapPixelToCoords({static_cast<int>(p.x()), static_cast<int>(p.y())});
        return {r.x, r.y};
    }

    std::optional<gld::Texture> Renderer::make_texture(const std::filesystem::path &filename) {
        sf::Texture texture;
        if ( bool success = texture.loadFromFile(filename) ) {
            auto [width, height] = texture.getSize();
            m_textures.emplace_back(std::move(texture));
            return gld::Texture{m_textures.size() - 1, {width, height}};
        } else {
            return {};
        }
    }

    const sf::Texture &Renderer::renderTarget() const {
        return m_renderTgt.getTexture();
    }

    void Renderer::update_view(const gld::Vector2f &size) {
        m_view.setSize({size.x(), size.y()});
        m_view.setCenter({0, 0}); // move the coordinate system origin to the center

        float factor = 1.0f;
        if ( size.x() > size.y() ) {
            factor = m_zoomTo.y() / size.y();
        } else {
            factor = m_zoomTo.x() / size.x();
        }
        m_view.zoom(factor);

        m_renderTgt.setView(m_view);
    }
} // gld::integration::SFML
