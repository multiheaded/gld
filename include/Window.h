//
// Created by marc on 27.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_WINDOW_H
#define GRIDLOCK_DEFENDERS_WINDOW_H

#include <memory>
#include "Event.h"

namespace gld {
    template <class WindowIntegrationT>
    class Window {
    public:
        Window(gld::Event::Broker &broker, unsigned int width, unsigned int height, const std::string &title)
            : m_impl(broker, width, height, title) {

        }

        void process_events() {
            m_impl.process_events();
        }

        template<typename RenderTargetT>
        void display(RenderTargetT &target) {
            m_impl.display(target);
        }

    private:
        WindowIntegrationT m_impl;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_WINDOW_H