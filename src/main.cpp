#include <Window.h>
#include <Game.h>
#include <Renderer.h>
#include <Event.h>
#include <Texture.h>
#include "integration/SFML/Renderer.h"
#include "integration/SFML/Window.h"
#include <archon/ecs.h>
#include <chrono>
#include "AssetManager.h"
#include "UI.h"
#include <functional>
#include <iomanip>

#include "Scheduler.h"

template<typename RendererIntegrationT, typename WindowIntegrationT>
void setup_frame_presentation(gld::EventBroker &broker, gld::Renderer<RendererIntegrationT> &renderer, gld::Window<WindowIntegrationT> &window);

template<typename RendererIntegrationT>
void setup_render_system(ecs::World &world, gld::EventBroker &broker, gld::Renderer<RendererIntegrationT> &renderer);

void setup_animation_system(ecs::World &world, gld::EventBroker &broker);

// composition root
int main() {
    ecs::World world;
    gld::register_components();
    gld::EventBroker broker;
    gld::Scheduler scheduler(broker);
    int cnt_200ms = 0;
    int cnt_500ms = 0;
    int cnt_1000ms = 0;
    auto task_200ms = [&cnt_200ms]() {
        std::cout << cnt_200ms++ << " 200ms\n";
    };
    auto task_1000ms = [&cnt_1000ms]() {
        std::cout << cnt_1000ms++ << " 1000ms\n";
    };
    scheduler.emplace(task_1000ms, std::chrono::milliseconds(1000));
    std::this_thread::sleep_for(std::chrono::milliseconds(1100));
    scheduler.emplace(task_200ms, std::chrono::milliseconds(200));
    gld::integration::SFML::Renderer renderer(broker);
    gld::integration::SFML::Window window(broker, 800, 600, "gld");
    gld::AssetManager assetManager(
        "../../assets/",
        "catalog.json",
        [&renderer](const std::filesystem::path &asset_name) -> std::optional<gld::Texture> {
                            return renderer.make_texture(asset_name);
                        }
    );

    gld::Game game(broker, world, assetManager);
    gld::UI ui(broker, world, assetManager);

    setup_frame_presentation(broker, renderer, window);

    setup_render_system(world, broker, renderer);
    setup_animation_system(world, broker);

    game.run();

    return 0;
}

template<typename RendererIntegrationT, typename WindowIntegrationT>
void setup_frame_presentation(gld::EventBroker &broker, gld::Renderer<RendererIntegrationT> &renderer, gld::Window<WindowIntegrationT> &window) {
    broker.subscribe<gld::PresentFrame>([&window, &renderer](const gld::PresentFrame &) {
        // the offscreen render target needs to be copied to the window
        window.display(renderer.renderTarget());
    });
}

template<typename RendererIntegrationT>
void setup_render_system(ecs::World &world, gld::EventBroker &broker, gld::Renderer<RendererIntegrationT> &renderer) {
    broker.subscribe<gld::Render>([&world, &renderer](const gld::Render &) {
        auto draw_fn = [&renderer](const gld::Drawable &d, const gld::Position &p) {
            renderer.draw(d.sprite, p.p);
        };
        // manual z-ordering using buckets
        ecs::Query<gld::Drawable, gld::Position>().with<gld::Draworder::Background>().each(world, draw_fn);
        ecs::Query<gld::Drawable, gld::Position>().with<gld::Draworder::Assets>().each(world, draw_fn);
        ecs::Query<gld::Drawable, gld::Position>().with<gld::Draworder::UI>().each(world, draw_fn);
    });
}

void setup_animation_system(ecs::World &world, gld::EventBroker &broker) {
    broker.subscribe<gld::UpdateAnimation>([&world](const gld::UpdateAnimation&) {
        auto current_time = std::chrono::steady_clock::now();
        ecs::Query<gld::Drawable, gld::Animated>().each(world, [current_time](gld::Drawable &d, gld::Animated &a) {
            // check whether this entity is due to be updated
            if (current_time>a.state.next_increment) {
                // advance frame with wraparound
                a.state.current_frame = (a.state.current_frame + 1) % d.sprite.animation.frame_count;

                // move the sprite rect to the position of the new current_frame
                d.sprite.rect.position = d.sprite.animation.origin + a.state.current_frame * d.sprite.animation.stride;

                // next update time is now() + time_increment
                a.state.next_increment = current_time + d.sprite.animation.frame_duration;
            }
        });
    });
}