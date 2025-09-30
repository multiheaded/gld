#ifndef GRIDLOCK_DEFENDERS_SCHEDULER_H
#define GRIDLOCK_DEFENDERS_SCHEDULER_H

#include <chrono>
#include <functional>
#include <thread>
#include <queue>
#include <vector>
#include <list>
#include <mutex>
#include <condition_variable>
#include <Event.h>

namespace gld {


    class Scheduler {
    public:
        using Callable = std::function<void()>;
        explicit Scheduler(gld::EventBroker &broker, std::uint32_t threads = std::thread::hardware_concurrency());

        void emplace(Callable task, std::chrono::milliseconds timeout);
    private:
        template <typename T>
        using priority_queue = std::priority_queue<T, std::vector<T>, std::greater<T>>;
        struct Task {
            std::uint8_t priority;
            Callable callable;
        };

        struct TimedTask {
            bool single_shot;
            std::chrono::milliseconds timeout;
            std::weak_ptr<Task> task;
        };

        struct ReadyTask {
            std::weak_ptr<Task> task;
        };

        struct ScheduleElement {
            TimedTask timed_task;
            std::chrono::steady_clock::time_point timepoint;
        };

        gld::EventBroker &m_broker;

        std::list<std::shared_ptr<Task>> m_tasks;
        priority_queue<ReadyTask> m_ready;
        priority_queue<ScheduleElement> m_schedule;
        std::vector<std::jthread> m_threads;
        std::jthread m_timer_thread;

        std::mutex m_mtx_ready;
        std::condition_variable m_cv_ready;

        std::mutex m_mtx_schedule;
        std::condition_variable m_cv_schedule;

        void run(std::stop_token stoken);
        void timer(std::stop_token stoken);
        void quit();



        friend bool operator>(const ScheduleElement& lhs, const ScheduleElement& rhs);
        friend bool operator>(const ReadyTask& lhs, const ReadyTask& rhs);
    };

    bool operator>(const Scheduler::ScheduleElement& lhs, const Scheduler::ScheduleElement& rhs);
    bool operator>(const Scheduler::ReadyTask& lhs, const Scheduler::ReadyTask& rhs);

}

#endif //GRIDLOCK_DEFENDERS_SCHEDULER_H