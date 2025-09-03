#include <Window.h>
#include <Game.h>
#include <Renderer.h>
#include <Event.h>
#include "integration/SFML/Renderer.h"
#include "integration/SFML/Window.h"
#include <archon/ecs.h>


int main() {
    gld::EventBroker broker;
    gld::integration::SFML::Renderer renderer(broker);
    gld::integration::SFML::Window window(broker, 800, 600, "gld");

    ecs::World world;
    ecs::register_component<gld::Position>();
    ecs::register_component<gld::Velocity>();
    ecs::register_component<gld::Drawable>();
    ecs::register_component<gld::Cursor>();

    gld::Game game(broker, world);

    auto cursor_entity = world.create_entity();
    world.add_components<gld::Drawable, gld::Position, gld::Cursor>(cursor_entity,
                                                                    {
                                                                        .sprite = renderer.make_sprite(
                                                                            "../../assets/head.png")
                                                                    },
                                                                    {},
                                                                    {});

    broker.subscribe<gld::PresentFrame>([&window, &renderer](const gld::PresentFrame &) {
        window.display(renderer.renderTarget());
    });

    broker.subscribe<gld::MouseMovedProjected>([&world](const gld::MouseMovedProjected& e) {
        ecs::Query<gld::Position>().with<gld::Cursor>().each(world, [&e](gld::Position &p) {
            p.p = e.p;
        });
    });

    broker.subscribe<gld::Render>([&world, &renderer](const gld::Render &) {
        ecs::Query<gld::Drawable, gld::Position>().each(world, [&renderer](const gld::Drawable &d, const gld::Position &p) {
            renderer.draw(d.sprite, p.p);
        });
    });

    game.run();

    return 0;
}
