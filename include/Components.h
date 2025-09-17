#ifndef GRIDLOCK_DEFENDERS_COMPONENTS_H
#define GRIDLOCK_DEFENDERS_COMPONENTS_H
#include <gld.h>
#include <memory>
#include "archon/ecs.h"
#include "Sprite.h"

namespace gld {
    struct Tile {
        std::size_t i, j;
    };
    struct Occupied {
        ecs::EntityId entity;
    };
    struct SpawnPoint {};
    struct Creep {};
    struct Tower {};

    struct Position {
        gld::Vector2f p;
    };

    struct Velocity {
        gld::Vector2f v;
    };

    struct Health {
        float value;
    };

    struct Drawable {
        Sprite sprite;
    };

    struct Animated {
        gld::AnimationState state;
    };

    struct Cursor{};

    namespace Draworder {
        struct Background{};
        struct Battlefield{};
        struct Assets{};
        struct UI{};
    }

    void register_components();
}

#endif //GRIDLOCK_DEFENDERS_COMPONENTS_H