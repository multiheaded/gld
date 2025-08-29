#ifndef GRIDLOCK_DEFENDERS_SFML_WINDOW_HPP
#define GRIDLOCK_DEFENDERS_SFML_WINDOW_HPP

#include <SFML/Graphics/Sprite.hpp>

template<typename RenderTargetT>
void gld::integration::SFML::Window::display(const RenderTargetT &target)
{
    m_window.clear();
    sf::Sprite tgtSprite(target);
    m_window.draw(tgtSprite);
    m_window.display();
}

#endif