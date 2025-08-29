//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_SFML_RENDERER_H
#define GRIDLOCK_DEFENDERS_SFML_RENDERER_H

#include <SFML/Graphics/RenderTexture.hpp>
#include <Event.h>
#include <Renderer.h>


namespace gld::integration::SFML {
    class Renderer : public gld::Renderer<gld::integration::SFML::Renderer> {
    public:
        explicit Renderer(gld::Event::Broker &broker);

        void resize(std::uint32_t width, std::uint32_t height);

        void render();

        const sf::Texture &renderTarget();

    private:
        sf::RenderTexture m_renderTgt;
        sf::View m_view;

        void update_view(float width, float height);
    };
} // gld::integration::SFML

#endif //GRIDLOCK_DEFENDERS_SFML_RENDERER_H
