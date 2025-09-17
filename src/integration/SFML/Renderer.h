//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_RENDERER_H
#define GRIDLOCK_DEFENDERS_SFML_RENDERER_H

#include <SFML/Graphics/RenderTexture.hpp>
#include <Event.h>
#include <Renderer.h>
#include <Sprite.h>
#include <Texture.h>


namespace gld::integration::SFML {
    class Renderer : public gld::Renderer<gld::integration::SFML::Renderer> {
    public:
        explicit Renderer(gld::EventBroker &broker);

        void resize(const gld::Vector2u &to_size);
        void zoom(const gld::Vector2f &to_size);

        void clear();
        void finish();

        void draw(const gld::Sprite &s, const gld::Vector2f &p);

        gld::Vector2u un_project(const gld::Vector2f &p) const;
        gld::Vector2f project(const gld::Vector2u &p) const;

        std::optional<gld::Texture> make_texture(const std::filesystem::path &filename);

        const sf::Texture &renderTarget() const;

    private:
        sf::RenderTexture m_renderTgt;
        sf::View m_view;
        std::vector<sf::Texture> m_textures;
        gld::Vector2f m_zoomTo;

        void update_view(const gld::Vector2f &size);
    };
} // gld::integration::SFML

#endif //GRIDLOCK_DEFENDERS_SFML_RENDERER_H
