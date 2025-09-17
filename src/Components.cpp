#include <Components.h>

namespace gld {
    void register_components() {
        ecs::register_component<gld::Position>();
        ecs::register_component<gld::Velocity>();
        ecs::register_component<gld::Drawable>();
        ecs::register_component<gld::Animated>();
        ecs::register_component<gld::Cursor>();
        ecs::register_component<gld::Draworder::Background>();
        ecs::register_component<gld::Draworder::Assets>();
        ecs::register_component<gld::Draworder::UI>();
    }
}