//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_EVENT_H
#define GRIDLOCK_DEFENDERS_EVENT_H
#include <cstdint>
#include <variant>
#include <optional>
#include <Eigen/Core>

#include "TypeContainer.h"
#include "PublishSubscribe.h"

namespace gld {
    template<typename... Es>
    struct Event;

    struct Resized {
        std::uint32_t width;
        std::uint32_t height;
    };

    struct MouseMoved {
        Eigen::Vector2i p;
    };

    struct MouseMovedProjected {
        Eigen::Vector2f p;
    };

    struct Quit {};

    struct ProcessUserInterfaceEvents {};

    using UIEvent = gld::Event<Resized, MouseMoved, MouseMovedProjected, Quit, ProcessUserInterfaceEvents>;

    struct BeginRenderPass {
    };

    struct Render {
    };

    struct EndRenderPass {
    };

    struct PresentFrame {
    };

    using RenderEvent = gld::Event<BeginRenderPass, Render, EndRenderPass, PresentFrame>;

    using EventBroker = gld::Broker<UIEvent, RenderEvent>;

    template<typename... Es>
    struct Event {
    public:
        using SubType = std::variant<Es...>;

        using Broker = gld::Broker<Event::SubType>;

        template<typename ExplicitEventType>
        explicit Event(const ExplicitEventType &event)
            : m_actualEvent(event)
            , m_subTypeIndex(gld::TypeContainer::index_of_v<SubType, ExplicitEventType>) {
        }

        template<typename ExplicitEventType>
        constexpr bool is() {
            return (m_subTypeIndex == gld::TypeContainer::index_of_v<SubType, ExplicitEventType>());
        }

        template<typename ExplicitEventType>
        constexpr std::optional<ExplicitEventType> as() {
            if constexpr (is<ExplicitEventType>()) {
                return std::optional<ExplicitEventType>(m_actualEvent);
            } else {
                return std::nullopt;
            }
        }

        template<typename VisitorT>
        constexpr decltype(auto) visit(VisitorT &&visitor) {
            return std::visit(std::forward<VisitorT>(visitor), m_actualEvent);
        }

    private:
        SubType m_actualEvent;
        std::size_t m_subTypeIndex;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_EVENT_H
