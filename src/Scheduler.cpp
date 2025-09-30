#include "Scheduler.h"

#include <chrono>

namespace gld {
    bool operator>(const Scheduler::ScheduleElement& lhs, const Scheduler::ScheduleElement& rhs) {
        return lhs.timepoint > rhs.timepoint;
    }

    bool operator>(const Scheduler::ReadyTask& lhs, const Scheduler::ReadyTask& rhs) {
        const auto lhsPtr = lhs.task.lock();
        const auto rhsPtr = rhs.task.lock();
        return lhsPtr->priority > rhsPtr->priority;
    }

    Scheduler::Scheduler(gld::EventBroker &broker, std::uint32_t threads)
        : m_broker(broker)
    {
        m_broker.subscribe<gld::Quit>([this](const gld::Quit&) {
            quit();
        });
        for (std::uint32_t i = 0; i < threads; i++) {
            m_threads.emplace_back(&Scheduler::run, this);
        }
        m_timer_thread = std::jthread(&Scheduler::timer, this);
    }

    void Scheduler::emplace(Callable task, std::chrono::milliseconds timeout) {
        const auto now = std::chrono::steady_clock::now();
        std::unique_lock lock(m_mtx_schedule);
        const std::shared_ptr<Task> task_ptr = std::make_shared<Task>(0, std::move(task));
        m_tasks.emplace_back(task_ptr);
        m_schedule.emplace(ScheduleElement{.timed_task={.single_shot=false, .timeout=timeout, .task=task_ptr}, .timepoint=now + timeout});
        m_cv_schedule.notify_one();
    }

    void Scheduler::run(std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            std::optional<ReadyTask> next;
            {
                std::unique_lock lock(m_mtx_ready);
                m_cv_ready.wait(lock, [&stoken, this](){ return (stoken.stop_requested() || !m_ready.empty()); });

                // gracefulle exit the loop
                if ( stoken.stop_requested() ) {
                    break;
                }

                next.emplace(m_ready.top());
                m_ready.pop();
            }
            m_cv_ready.notify_all();

            auto task = next->task.lock();
            task->callable();
        }
    }

    void Scheduler::timer(std::stop_token stoken) {
        while (!stoken.stop_requested()) {
            // get or wait for the next task in the schedule
            ScheduleElement next;
            {
                auto schedule_ready = [&stoken, this]() -> bool {
                    const bool stop_requested = stoken.stop_requested();
                    const bool schedule_has_elements = !m_schedule.empty();
                    return stop_requested || schedule_has_elements;
                };

                // wait for stop request or a new schedule element
                std::unique_lock lock(m_mtx_schedule);
                if ( m_schedule.empty() ) {
                    m_cv_schedule.wait(lock, schedule_ready);

                    // gracefully exit the loop
                    if ( stoken.stop_requested() ) {
                        break;
                    }
                }

                assert(!m_schedule.empty()); // this should not happen as per predicate can_proceed

                auto next_timepoint = m_schedule.top().timepoint;
                auto wait_complete_or_rescheduled = [&stoken, &next_timepoint, this]() -> bool {
                    const bool stop_requested = stoken.stop_requested();
                    const bool next_tp_changed = next_timepoint != m_schedule.top().timepoint;
                    return stop_requested || next_tp_changed;
                };
                m_cv_schedule.wait_until(lock, next_timepoint, wait_complete_or_rescheduled);

                // gracefully exit the loop
                if ( stoken.stop_requested() ) {
                    break;
                }

                // the order of the schedule has changed, e.g. due to removal or addition of tasks
                if ( next_timepoint != m_schedule.top().timepoint ) {
                    continue; // wait_again, this time until the new timepoint is reached
                }

                // get the next schedule element
                next = m_schedule.top();
                m_schedule.pop();
            }

            // enque the task in the ready queue for further processing and notify worker threads to pick up the task
            {
                std::unique_lock lock(m_mtx_ready);
                m_ready.emplace(ReadyTask{next.timed_task.task});
            }
            m_cv_ready.notify_all();

            // calculate the next execution timepoint and enqueue the schedule element
            if (!next.timed_task.single_shot) {
                next.timepoint += next.timed_task.timeout;
                {
                    std::unique_lock lock(m_mtx_schedule);
                    m_schedule.emplace(next);
                }
            }
        }
    }

    void Scheduler::quit() {
        m_timer_thread.request_stop();
        for (auto& worker : m_threads) {
            worker.request_stop();
        }
        m_cv_schedule.notify_all();
        m_cv_ready.notify_all();
    }
}
