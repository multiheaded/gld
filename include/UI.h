#ifndef GRIDLOCK_DEFENDERS_UI_H
#define GRIDLOCK_DEFENDERS_UI_H
#include "AssetManager.h"
#include "Event.h"
#include "archon/ecs.h"

namespace gld {
    class UI {
    public:
        explicit UI(gld::EventBroker &broker, ecs::World& world, const AssetManager &asset_manager);

        void create_cursor() const;

    private:
        gld::EventBroker &m_broker;
        ecs::World &m_world;
        const AssetManager &m_asset_manager;
    };
}


#endif //GRIDLOCK_DEFENDERS_UI_H