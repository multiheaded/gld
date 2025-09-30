#ifndef GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#define GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#include <functional>
#include <thread>
#include <cassert>
#include <deque>
#include <shared_mutex>
#include <mutex>
#include <iostream>

#define assertm(exp, msg) assert((void(msg), exp))

namespace gld {
    enum class PublicationType {Direct, Queued};

    template<typename EventT>
    class Broker {
    public:
        template<typename T>
        using CallbackT = std::function<void(const T&)>;

        template<typename T>
        struct Subscriber {
            CallbackT<T> callback;
            std::thread::id thread_id;

            mutable std::mutex inbox_mutex;
            std::deque<T> inbox;

            Subscriber(CallbackT<T> cb)
                : callback(std::move(cb))
                , thread_id{std::this_thread::get_id()} {
            }

            Subscriber(const Subscriber &other)
                : callback(other.callback)
                , thread_id(other.thread_id) {
                std::scoped_lock other_lock(other.inbox_mutex);
                std::scoped_lock inbox_lock(inbox_mutex);
                std::copy(other.inbox.begin(), other.inbox.end(), std::back_inserter(inbox));
            }

            Subscriber(Subscriber &&other) noexcept
                : callback(std::move(other.callback))
                , thread_id(std::move(other.thread_id)) {
                std::scoped_lock other_lock(other.inbox_mutex);
                std::scoped_lock inbox_lock(inbox_mutex);
                inbox.swap(other.inbox);
            }

            Subscriber& operator=(const Subscriber& other) {
                callback = other.callback;
                thread_id = other.thread_id;
                std::scoped_lock other_lock(other.inbox_mutex);
                std::scoped_lock inbox_lock(inbox_mutex);
                std::copy(other.inbox.begin(), other.inbox.end(), std::back_inserter(inbox));
                return *this;
            }
            Subscriber& operator=(Subscriber&& other) noexcept {
                callback = std::move(other.callback);
                thread_id = std::move(other.thread_id);
                std::scoped_lock other_lock(other.inbox_mutex);
                std::scoped_lock inbox_lock(inbox_mutex);
                inbox.swap(other.inbox);
                return *this;
            }
        };

        template<typename T>
        using SubscriberVectorT = std::vector<Subscriber<T>>;

        template<typename T, typename CallbackT>
        void subscribe(CallbackT &&callback) {
            std::unique_lock writer_lock(m_subscribers_mutex);
            auto& subs = subscribers<T>();
            subs.emplace_back(std::forward<CallbackT>(callback));
        }

        template<typename T>
        void publish(const T& val, PublicationType type = gld::PublicationType::Direct) {
            std::shared_lock read_lock(m_subscribers_mutex);
            auto& subs = subscribers<T>();
            for (auto& sub: subs) {
                if (sub.thread_id == std::this_thread::get_id() && type == gld::PublicationType::Direct) {
                    sub.callback(val);
                } else {
                    // enque an event into the subscriber's inbox
                    std::scoped_lock inbox_lock(sub.inbox_mutex);
                    sub.inbox.emplace_back(val);
                }
            }
        }

        void process() {
            std::shared_lock read_lock(m_subscribers_mutex);
            std::apply([this](auto&... vecs) {
                std::thread::id my_thread_id = std::this_thread::get_id();
                (drain_queues(vecs, my_thread_id), ...);
            }, m_subscribers);
        }

    private:
        template<typename C>
        struct type_map;

        template<template<typename...> typename C, typename... Ps>
        struct type_map<C<Ps...>> {
            using subscribers_type = std::tuple<SubscriberVectorT<Ps>...>;
        };

        // map type index to subscribers via tuple:
        type_map<EventT>::subscribers_type m_subscribers;
        std::shared_mutex m_subscribers_mutex;

        template<typename T>
        auto& subscribers() {
            return std::get<SubscriberVectorT<T>>(m_subscribers);
        }

        template<typename T>
        void drain_queues(SubscriberVectorT<T> &subs, std::thread::id tid) {
            for (Subscriber<T>& sub: subs) {
                if (sub.thread_id == tid) {
                    std::deque<T> local_q;
                    {
                        std::scoped_lock inbox_lock(sub.inbox_mutex);
                        local_q.swap(sub.inbox);
                    }
                    for (auto& val: local_q) {
                        sub.callback(val);
                    }
                }
            }
        }
    };


} // gld

#endif //GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H