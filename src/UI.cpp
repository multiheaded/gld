#include <UI.h>
#include <Components.h>

namespace gld {
    UI::UI(gld::EventBroker& broker, ecs::World &world, const AssetManager &asset_manager)
        : m_broker(broker)
        , m_world(world)
        , m_asset_manager(asset_manager)
    {
        create_cursor();
        broker.subscribe<gld::MouseMovedProjected>([&world](const gld::MouseMovedProjected& e) {
            ecs::Query<gld::Position>().with<gld::Cursor>().each(world, [&e](gld::Position &p) {
                p.p = e.p;
            });
        });
    }

    void UI::create_cursor() const {
        auto cursor_entity = m_world.create_entity();
        auto blob_sprite = m_asset_manager.get<gld::Sprite>(gld::Asset::SpriteCursorAnimated);

        if (blob_sprite) {
            m_world.add_components<gld::Drawable, gld::Animated, gld::Position, gld::Cursor, gld::Draworder::UI>(
                cursor_entity,
                {
                    .sprite = blob_sprite.value()
                },
                {
                    .state = {}
                },
                {},
                {},
                {});
        }
    }
}
