//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_GAME_H
#define GRIDLOCK_DEFENDERS_GAME_H

#include <iostream>

#include "Event.h"
#include <queue>

namespace gld {
    template <typename WindowT, typename RendererT>
    class Game {
    public:
        Game(gld::Event::Broker &broker, WindowT &wnd, RendererT &renderer)
            : m_broker(broker)
            , m_wnd(wnd)
            , m_renderer(renderer) {
            broker.subscribe<gld::Event::Quit>([this](const gld::Event::Quit&){quit();});
        }

        void run() {
            m_running = true;
            while (m_running) {
                m_wnd.process_events();
                m_renderer.render();
                m_wnd.display(m_renderer.renderTarget());
            }
        }

        void quit() {
            m_running = false;
        }
    private:
        bool m_running = false;
        gld::Event::Broker &m_broker;
        WindowT &m_wnd;
        RendererT &m_renderer;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_GAME_H