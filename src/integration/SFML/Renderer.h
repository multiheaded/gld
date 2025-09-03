//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_RENDERER_H
#define GRIDLOCK_DEFENDERS_SFML_RENDERER_H

#include <Eigen/Core>
#include <SFML/Graphics/RenderTexture.hpp>
#include <Event.h>
#include <Renderer.h>
#include <Sprite.h>


namespace gld::integration::SFML {
    class Renderer : public gld::Renderer<gld::integration::SFML::Renderer> {
    public:
        explicit Renderer(gld::EventBroker &broker);

        void resize(std::uint32_t width, std::uint32_t height);

        void clear();
        void finish();

        void draw(const gld::Sprite &s, const Eigen::Vector2f &p);

        Eigen::Vector2i un_project(Eigen::Vector2f p);
        Eigen::Vector2f project(Eigen::Vector2i p);

        gld::Sprite make_sprite(const std::string &filename);

        const sf::Texture &renderTarget() const;

    private:
        sf::RenderTexture m_renderTgt;
        sf::View m_view;
        std::vector<sf::Texture> m_textures;

        void update_view(float width, float height);
    };
} // gld::integration::SFML

#endif //GRIDLOCK_DEFENDERS_SFML_RENDERER_H
