#ifndef GRIDLOCK_DEFENDERS_SPRITE_H
#define GRIDLOCK_DEFENDERS_SPRITE_H
#include <cstddef>
#include <gld.h>
#include <chrono>

namespace gld {
    struct Sprite {
        std::size_t id;
        UnsignedRect rect;
        Vector2u center;

        struct Animation {
            gld::Vector2u origin;
            gld::Vector2u stride;
            std::chrono::milliseconds frame_duration;
            std::size_t frame_count;
        } animation;
    };

    struct AnimationState {
        std::chrono::steady_clock::time_point next_increment;
        std::size_t current_frame;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_SPRITE_H