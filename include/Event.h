//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_EVENT_H
#define GRIDLOCK_DEFENDERS_EVENT_H
#include <cstdint>
#include <variant>
#include <optional>
#include "TypeContainer.h"
#include "PublishSubscribe.h"

namespace gld {
    struct Event {
    public:
        struct Resized {
            std::uint32_t width;
            std::uint32_t height;
        };

        struct Quit {};

        using SubType = std::variant<Resized, Quit>;
        using Broker = gld::Broker<Event::SubType>;

        template<typename ExplicitEventType>
        explicit Event(const ExplicitEventType &event)
            : m_actualEvent(event)
            , m_subTypeIndex(gld::TypeContainer::index_of_v<SubType, ExplicitEventType>())
        {}

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

    private:
        SubType m_actualEvent;
        std::size_t m_subTypeIndex;
    };

} // gld

#endif //GRIDLOCK_DEFENDERS_EVENT_H