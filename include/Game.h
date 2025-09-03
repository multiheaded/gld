//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_GAME_H
#define GRIDLOCK_DEFENDERS_GAME_H

#include "Event.h"
#include "Components.h"
#include <archon/ecs.h>

namespace gld {
    class Game {
    public:
        Game(gld::EventBroker &broker, ecs::World &world)
            : m_broker(broker)
            , m_world(world)
        {
            broker.subscribe<gld::Quit>([this](const gld::Quit&) {
                quit();
            });
        }

        void run() {
            m_running = true;
            while (m_running) {
                m_broker.publish(gld::ProcessUserInterfaceEvents{});
                m_broker.publish(gld::BeginRenderPass{});
                m_broker.publish(gld::Render{});
                m_broker.publish(gld::EndRenderPass{});
                m_broker.publish(gld::PresentFrame{});
            }
        }

        void quit() {
            m_running = false;
        }
    private:
        bool m_running = false;
        gld::EventBroker &m_broker;
        ecs::World &m_world;

        void add_spawn_point(gld::Position pos) {
            auto entity = m_world.create_entity();
            m_world.add_components(entity, gld::SpawnPoint{}, pos);
        }

        void build_tower(gld::Position pos) {
            auto entity = m_world.create_entity();
            m_world.add_components(entity, gld::Tower{}, pos);
        }

        void spawn_creep(gld::Position pos, gld::Velocity vel, gld::Health health) {
            auto entity = m_world.create_entity();
            m_world.add_components(entity, gld::Creep{}, pos, vel, health);
        }

        void move_creeps() {
            ecs::Query<gld::Creep, gld::Position, gld::Velocity>().each(m_world, [](gld::Creep&, gld::Position& pos, gld::Velocity& vel) {
                pos.p += vel.v;
            });
        }
    };
} // gld

#endif //GRIDLOCK_DEFENDERS_GAME_H