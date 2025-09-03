#ifndef GRIDLOCK_DEFENDERS_TYPECONTAINER_H
#define GRIDLOCK_DEFENDERS_TYPECONTAINER_H

#include <type_traits>

namespace gld {
    namespace TypeContainer {
        namespace contains_traits {
            template<typename Container, typename T>
            struct contains : std::false_type {};

            template<template<typename...> typename Container, typename T, typename... Ps>
            struct contains<Container<Ps...>, T> : std::disjunction<std::is_same<T, Ps>...> {
            };
        }

        template<typename Container, typename T>
        using contains = contains_traits::contains<Container, T>;

        template<typename Container, typename T>
        inline constexpr bool contains_v = contains_traits::contains<Container, T>::value;

        namespace concat_traits {
            template<typename C1, typename C2>
            struct concat_two;

            template<template<typename...> typename C1, template<typename...> typename C2, typename... T1s, typename... T2s>
            struct concat_two<C1<T1s...>, C2<T2s...>> {
                using type = C1<T1s..., T2s...>;
            };

            template<typename C1, typename C2>
            using concat_two_t = typename concat_two<C1, C2>::type;

            template<typename... Cs>
            struct concat_n;

            template<typename C>
            struct concat_n<C> {
                using type = C;
            };

            template<typename C1, typename C2, typename... CRest>
            struct concat_n<C1, C2, CRest...> {
                using type = typename concat_n<concat_two_t<C1, C2>, CRest...>::type;
            };
        }

        template<typename... Container>
        using concat_t = typename concat_traits::concat_n<Container...>::type;

        namespace test {
            using C1 = std::tuple<int, float>;
            using C2 = std::tuple<long, double>;
            using C3 = std::tuple<bool>;
            static_assert(std::is_same_v<concat_t<C1,C2>, std::tuple<int, float, long, double>> == true);
            static_assert(std::is_same_v<concat_t<C1,C2,C3>, std::tuple<int, float, long, double, bool>> == true);
        }

        namespace unique_traits {
            template<typename T, typename... Ps>
            struct unique;

            template<typename T>
            struct unique<T> {
                using type = T;
            };

            template<template<typename...> typename Container, typename... Ts, typename P, typename... Ps>
            struct unique<Container<Ts...>, P, Ps...> {
                using type = std::conditional_t<
                        contains_v<Container<Ts...>, P>, //if first of P is in pack Ts the disjunction returns true, P already is in container
                        typename unique<Container<Ts...>, Ps...>::type,
                        typename unique<Container<Ts..., P>, Ps...>::type
                >;

                //idea:
                // recursively check pack Ps for duplicates, starting with the first and continuing with the rest
                // first P of Ps not yet in container: add P to the container and continue with the rest of Ps
                // first P of Ps already in container: ignore P and continue with the rest of Ps
            };

            template<typename Container>
            struct make_unique;

            template<template <typename...> typename Container, typename... Ts>
            struct make_unique<Container<Ts...>> {
                using type = typename unique<Container<>, Ts...>::type;
            };
        }

        template<typename Container>
        using make_unique = unique_traits::make_unique<Container>;

        template<typename Container>
        using make_unique_t = typename make_unique<Container>::type;

        namespace container_index_traits {
            template<typename P, typename I>
            struct container_index_helper;

            template<template<typename...> typename P, typename... Ps, std::size_t... Is>
            struct container_index_helper<P<Ps...>, std::index_sequence<Is...>> {
                template<typename Q, typename K, std::size_t V>
                struct tuple_index_query : std::bool_constant<std::is_same_v<Q, K>> {
                    static constexpr std::size_t index = V;
                };

                template<typename T>
                struct of {
                    using IndexOf = std::disjunction<tuple_index_query<T, Ps, Is>...>;
                    static constexpr std::size_t value = IndexOf::index;
                };
            };

            template<typename P>
            struct container_index;

            template<template<typename...> typename P, typename...Ps>
            struct container_index<P<Ps...>> : container_index_helper<P<Ps...>, std::make_index_sequence<sizeof...(Ps)>> {};
        }

        namespace container_index_traits {
            template<typename Container>
            struct container_type;

            template<template<typename ...> typename P, typename... Ps>
            struct container_type<P<Ps...>> {
                template<std::size_t Idx, typename P0, typename... PRest>
                struct Nth_type {
                    using type = Nth_type<Idx-1, PRest...>::type;
                };

                template<typename P0, typename... Po>
                struct Nth_type<0, P0, Po...> {
                    using type = P0;
                };

                template<std::size_t N>
                struct at {
                    using type = typename Nth_type<N, Ps...>::type;
                };
            };
        }


        template<typename Container, typename T>
        struct index_of {
            static constexpr std::size_t value = container_index_traits::container_index<Container>::template of<T>::value;
        };

        template<typename Container, typename T>
        inline constexpr std::size_t index_of_v = index_of<Container, T>::value;

        template<typename Container, std::size_t Index>
        struct type_at {
            using type = container_index_traits::container_type<Container>::template at<Index>::type;
        };

        template<typename Container, std::size_t Index>
        using type_at_t = type_at<Container, Index>::type;

        namespace test {
            using C = std::variant<int, float, double, bool, void>;
            static_assert(std::is_same_v<type_at_t<C, 2>, double> == true);
        }
    }
}

#endif //GRIDLOCK_DEFENDERS_TYPECONTAINER_H
