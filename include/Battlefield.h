#ifndef GRIDLOCK_DEFENDERS_BATTLEFIELD_H
#define GRIDLOCK_DEFENDERS_BATTLEFIELD_H
#include <archon/ecs.h>
#include <gld.h>
#include <Texture.h>


namespace gld {
    class Battlefield {
    public:
        Battlefield(ecs::World &world, gld::Texture texture, gld::Vector2u tiling);

        gld::Vector2u size() const;

    private:
        ecs::World &m_world;
        gld::Texture m_texture;
        gld::Vector2u m_tiling;
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_BATTLEFIELD_H