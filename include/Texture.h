#ifndef GRIDLOCK_DEFENDERS_TEXTURE_H
#define GRIDLOCK_DEFENDERS_TEXTURE_H
#include <cstddef>
#include <utility>
#include <gld.h>

namespace gld {
    class Texture {
    public:
        std::size_t id;
        gld::Vector2u size;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_TEXTURE_H