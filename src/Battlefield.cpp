#include <Battlefield.h>
#include <Components.h>

namespace gld {
    Battlefield::Battlefield(ecs::World &world, gld::Texture texture, gld::Vector2u tiling)
        : m_world(world)
          , m_texture(texture)
          , m_tiling(tiling) {
        ecs::EntityId battlefield_entity = world.create_entity();
        gld::Sprite sprite{
            .id = m_texture.id,
            .rect = {
                .position = {0, 0},
                .size = m_texture.size
            },
            .center = m_texture.size / 2
        };
        world.add_components<gld::Drawable, gld::Position, gld::Draworder::Background>(
            battlefield_entity, {.sprite = sprite}, {}, {});
    }

    gld::Vector2u Battlefield::size() const {
        return m_texture.size;
    }
} // gld
