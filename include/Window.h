//
// Created by marc on 27.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_WINDOW_H
#define GRIDLOCK_DEFENDERS_WINDOW_H

#include <iostream>
#include <memory>
#include "Event.h"

namespace gld {
    template <class WindowIntegrationT>
    class Window {
    public:
        Window(gld::Event::Broker &broker, unsigned int, unsigned int, const std::string&)
            : m_broker(broker)
        {
            std::cout << "Base Window created" << std::endl;
        }

        void process_events() {
            derived().process_events();
        }

        template<typename RenderTargetT>
        void display(RenderTargetT &target) {
            derived().display(target);
        }

        constexpr WindowIntegrationT &derived() {
            return static_cast<WindowIntegrationT &>(*this);
        }

    protected:
        gld::Event::Broker &m_broker;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_WINDOW_H