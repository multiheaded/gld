#ifndef GRIDLOCK_DEFENDERS_EVENT_H
#define GRIDLOCK_DEFENDERS_EVENT_H
#include <variant>
#include <optional>

#include "ConcurrentQueue.h"
#include "gld.h"
#include "PublishSubscribe.h"
#include "TypeContainer.h"

namespace gld {
    template<typename... Es>
    struct Event;

    struct Resized {
        gld::Vector2u size;
    };

    struct MouseMoved {
        gld::Vector2u p;
    };

    struct MouseMovedProjected {
        gld::Vector2f p;
    };

    struct Quit {};

    struct ProcessUserInterfaceEvents {};

    using UIEvent = gld::Event<Resized, MouseMoved, MouseMovedProjected, Quit, ProcessUserInterfaceEvents>;

    struct UpdateAnimation {};

    using AnimationEvent = gld::Event<UpdateAnimation>;

    struct BeginRenderPass {
    };

    struct Render {
    };

    struct EndRenderPass {
    };

    struct PresentFrame {
    };

    struct ViewZoomToFit {
        gld::Vector2f viewSize;
    };

    using RenderEvent = gld::Event<BeginRenderPass, Render, EndRenderPass, PresentFrame, ViewZoomToFit>;

    using Events = gld::TypeContainer::concat_t<UIEvent, RenderEvent, AnimationEvent>;

    using EventBroker = gld::Broker<Events>;

    using EventQueue = gld::ConcurrentQueue<Events>;

    template<typename... Es>
    struct Event {
    public:
        using SubType = std::variant<Es...>;

        template<typename ExplicitEventType>
        explicit constexpr Event(const ExplicitEventType &event)
            : m_actualEvent(event) {
        }

        template<typename ExplicitEventType>
        constexpr bool is() {
            return (std::holds_alternative<ExplicitEventType>(m_actualEvent));
        }

        template<typename ExplicitEventType>
        constexpr std::optional<ExplicitEventType> as() {
            if constexpr (std::holds_alternative<ExplicitEventType>(m_actualEvent)) {
                return std::optional<ExplicitEventType>(std::get<ExplicitEventType>(m_actualEvent));
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
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_EVENT_H
