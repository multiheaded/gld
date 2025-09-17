#ifndef GRIDLOCK_DEFENDERS_CONCURRENTQUEUE_H
#define GRIDLOCK_DEFENDERS_CONCURRENTQUEUE_H

#include <queue>
#include <mutex>
#include <vector>
#include <condition_variable>
#include <functional>

namespace gld {
    template<typename C>
    class ConcurrentQueue{
    public:
        void put(C ev) {
            std::unique_lock lock(mutex);
            objs.push(C{ev});
        }

        void put(std::vector<C> evs) {
            std::unique_lock lock(mutex);
            for(C &ev : evs) {
                objs.push(std::move(ev));
            }
        }

        template<typename FuncT>
        void drain_and_apply(FuncT &&func) {
            // block until objects arrive
            std::unique_lock lock(mutex);
            while( !objs.empty() ) {
                C obj = std::move(objs.front());
                objs.pop();

                std::invoke(std::forward<FuncT>(func), obj);
            }
        }

    private:
        std::queue<C> objs;
        std::mutex mutex;
    };
}

#endif //GRIDLOCK_DEFENDERS_CONCURRENTQUEUE_H