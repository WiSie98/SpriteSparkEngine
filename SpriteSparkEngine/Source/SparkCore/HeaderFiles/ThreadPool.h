#pragma once

#include "Sparkpch.h"

namespace SpriteSpark {

    // ThreadPool Klasse zur Verwaltung von Worker-Threads und Aufgabenwarteschlange
    class ThreadPool {
    public:

        ThreadPool(size_t numThreads = std::thread::hardware_concurrency());

        ~ThreadPool();

        // Fügt eine neue Aufgabe in die Warteschlange ein und gibt ein future-Objekt zurück
        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
            using returnType = typename std::invoke_result<F, Args...>::type;

            auto task = std::make_shared<std::packaged_task<returnType()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));

            std::future<returnType> res = task->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);

                // Warten, bis ein Worker verfügbar wird
                condition.wait(lock, [this] { return stop || activeTasks < workers.size(); });

                if (stop) throw std::runtime_error("enqueue on stopped ThreadPool");

                tasks.emplace([task, this]() {
                    try {
                        (*task)();
                    }
                    catch (const std::exception& e) {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        std::cerr << "Task exception: " << e.what() << std::endl;
                    }
                    catch (...) {
                        std::lock_guard<std::mutex> lock(queueMutex);
                        std::cerr << "Task exception: unknown exception" << std::endl;
                    }
                    --activeTasks;
                    if (tasks.empty() && activeTasks == 0) {
                        completionCondition.notify_all();
                    }
                    condition.notify_one();
                    });
                ++activeTasks;
            }
            condition.notify_one();
            return res;
        }

        // Wartet, bis alle Aufgaben abgeschlossen sind
        void waitForCompletion();

        // Wartet für eine bestimmte Zeit, bis alle Aufgaben abgeschlossen sind
        bool waitForCompletionFor(size_t milliseconds);

        // Passt die Anzahl der Threads zur Laufzeit an
        void resize(size_t numThreads);

        void stopAll();

    private:
        void workerThread();

        std::vector<std::thread> workers;
        std::queue<std::function<void()>> tasks;

        std::mutex queueMutex;
        std::condition_variable condition;
        std::condition_variable completionCondition;
        std::atomic<bool> stop;
        std::atomic<int> activeTasks;
    };

}