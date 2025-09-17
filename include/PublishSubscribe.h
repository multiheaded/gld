#ifndef GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#define GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#include <functional>
#include <thread>
#include <cassert>

#define assertm(exp, msg) assert((void(msg), exp))

namespace gld {
    enum class PublicationType {Direct, Queued};

    template<typename EventT>
    class Broker {
    public:

        template<typename T>
        using SubscriberVectorType = std::vector<std::function<void(const T&)>>;

        template<typename T>
        using QueueType = gld::ConcurrentQueue<T>;

        Broker()
            : m_constructor_thread_id(std::this_thread::get_id())
        {}

        template<typename T, typename CallbackT>
        void subscribe(CallbackT &&callback) {
            assertm(std::this_thread::get_id() == m_constructor_thread_id, "only subscriptions from the owning thread are allowed");
            std::get<SubscriberVectorType<T>>(m_subscribers).push_back(std::forward<CallbackT>(callback));
        }

        template<typename T>
        void publish(const T& val, PublicationType type = gld::PublicationType::Direct) {
            // direct notification of subscribers is only allowed if the PublicationType is Direct
            //  and if publish was called from the owning thread
            if ( std::this_thread::get_id() == m_constructor_thread_id && gld::PublicationType::Direct == type ) {
                notify_all<T>(val);
            } else {
                // all other calls to publish will be queued
                std::get<QueueType<T>>(m_queues).put(val);
            }
        }

        void process() {
            // make sure subscribers are only called from the owning thread
            assertm(std::this_thread::get_id() == m_constructor_thread_id, "only the owning thread may drain the queues");
            drain_queues();
        }

    private:
        template<typename C>
        struct type_map;

        template<template<typename...> typename C, typename... Ps>
        struct type_map<C<Ps...>> {
            using subscribers_type = std::tuple<SubscriberVectorType<Ps>...>;
            using queue_type = std::tuple<QueueType<Ps>...>;
        };

        // map type index to subscribers via tuple:
        type_map<EventT>::subscribers_type m_subscribers;
        type_map<EventT>::queue_type m_queues;
        std::thread::id m_constructor_thread_id;

        template <typename T>
        void notify_all(const T &val) {
            for (const auto& subscriber : std::get<SubscriberVectorType<T>>(m_subscribers)) {
                subscriber(val);
            }
        }

        void drain_queues() {
            std::apply([this]<typename... T>(QueueType<T>&... queues) {
                auto drain = [this]<typename S>(QueueType<S>& queue){
                    //queue.drain_and_apply(std::bind_front(std::mem_fn(&Broker<EventT>::notify_all<S>), this));
                    queue.drain_and_apply([this](const auto &obj) {
                        notify_all(obj);
                    });
                };
                (drain(queues),...);
            }, m_queues);
        }

    };


} // gld

#endif //GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H