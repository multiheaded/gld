//
// Created by marc on 01.09.25.
//

#ifndef GRIDLOCK_DEFENDERS_COMPONENTS_H
#define GRIDLOCK_DEFENDERS_COMPONENTS_H
#include <Eigen/Core>
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
        Eigen::Vector2f p;
    };

    struct Velocity {
        Eigen::Vector2f v;
    };

    struct Health {
        float value;
    };

    struct Drawable {
        Sprite sprite;
    };

    struct Cursor{};
}

#endif //GRIDLOCK_DEFENDERS_COMPONENTS_H