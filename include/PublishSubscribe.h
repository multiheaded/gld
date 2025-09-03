//
// Created by marc on 28.08.25.
//

#ifndef GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#define GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H
#include <functional>
#include <memory>
#include "TypeContainer.h"

namespace gld {
    template<typename... EventTs>
    class Broker {
    private:
        template<typename T>
        struct Subscriber {
            explicit Subscriber(std::function<void(const T&)> callback) : notify(std::move(callback)) {}
            std::function<void(const T&)> notify;
        };
    public:
        using SubType = gld::TypeContainer::concat_t<EventTs...>;

        template<typename T, typename CallbackT>
        void subscribe(CallbackT &&callback) {
            using SubscriberVectorType = std::vector<std::unique_ptr<Subscriber<T>>>;
            std::unique_ptr<Subscriber<T>> subscriber = std::make_unique<Subscriber<T>>(std::forward<CallbackT>(callback));
            std::get<SubscriberVectorType>(m_subscribers).push_back(std::move(subscriber));
        }

        template<typename T>
        void publish(const T& val) {
            using SubscriberVectorType = std::vector<std::unique_ptr<Subscriber<T>>>;
            for (const std::unique_ptr<Subscriber<T>>& subscriber : std::get<SubscriberVectorType>(m_subscribers)) {
                subscriber->notify(val);
            }
        }
    private:
        template<typename C>
        struct type_map;

        template<template<typename...> typename C, typename... Ps>
        struct type_map<C<Ps...>> {
            using type = std::tuple<std::vector<std::unique_ptr<Subscriber<Ps>>>...>;
        };

        // map type index to subscribers via tuple:
        typename type_map<SubType>::type m_subscribers;
    };


} // gld

#endif //GRIDLOCK_DEFENDERS_PUBLISHSUBSCRIBE_H