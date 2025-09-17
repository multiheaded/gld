#include "Game.h"

namespace gld {
    Game::Game(gld::EventBroker &broker, ecs::World &world, gld::AssetManager &assetManager)
        : m_broker(broker)
        , m_world(world)
        , m_assetManager(assetManager)
        , m_battlefield(
                world,
                assetManager.get<gld::Texture>(gld::Asset::Battlefield).value(),
                {0,0}
            )
    {
        broker.subscribe<gld::Quit>([this](const gld::Quit&) {
            quit();
        });

        broker.publish<gld::ViewZoomToFit>({
            .viewSize{m_battlefield.size().cast<float>()}
        });
    }

} // gld