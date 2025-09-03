//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_RENDERER_H
#define GRIDLOCK_DEFENDERS_RENDERER_H

#include <Event.h>
#include <Sprite.h>

namespace gld {
    template <class RendererIntegrationT>
    class Renderer {
    public:
        Renderer() = delete;
        explicit Renderer(gld::EventBroker &broker) : m_broker(broker) {
            broker.subscribe<gld::Resized>([this](const gld::Resized& val) {
                    derived().resize(val.width, val.height);
                });
            broker.subscribe<gld::MouseMoved>([this](const gld::MouseMoved& val) {
                Eigen::Vector2f projected = derived().project(val.p);
                m_broker.publish(gld::MouseMovedProjected{
                    projected
                });
            });
            broker.subscribe<gld::BeginRenderPass>([this](const auto&) {
                begin_render_pass();
            });
            broker.subscribe<gld::EndRenderPass>([this](const auto&) {
                end_render_pass();
            });
        };

        void begin_render_pass() {
            derived().clear();
        }

        void end_render_pass() {
            derived().finish();
        }

        void draw(const gld::Sprite &s, const Eigen::Vector2f &p) {
            derived().draw(s, p);
        }

        gld::Sprite make_sprite() {
            return derived().make_sprite();
        }

        const auto& renderTarget() {
            return derived().renderTarget();
        }

        constexpr RendererIntegrationT &derived() {
            return static_cast<RendererIntegrationT &>(*this);
        }

    protected:
        gld::EventBroker &m_broker;
    };
}



#endif //GRIDLOCK_DEFENDERS_RENDERER_H