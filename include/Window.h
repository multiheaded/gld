#ifndef GRIDLOCK_DEFENDERS_WINDOW_H
#define GRIDLOCK_DEFENDERS_WINDOW_H

#include <iostream>
#include <memory>
#include "Event.h"

namespace gld {
    template <class WindowIntegrationT>
    class Window {
    public:
        Window(gld::EventBroker &broker, unsigned int, unsigned int, const std::string&)
            : m_broker(broker)
        {
            broker.subscribe<gld::ProcessUserInterfaceEvents>([this](const auto&) {
                process_events();
            });
        }

        void process_events() {
            while (std::optional event = derived().query_event() ) {
                gld::UIEvent ev = event.value();
                auto visitor = [this](auto v) {
                        m_broker.publish(v);
                    };
                ev.visit(visitor);
            }
        }

        template<typename RenderTargetT>
        void display(RenderTargetT &target) {
            derived().display(target);
        }

        constexpr WindowIntegrationT &derived() {
            return static_cast<WindowIntegrationT &>(*this);
        }

    protected:
        gld::EventBroker &m_broker;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_WINDOW_H