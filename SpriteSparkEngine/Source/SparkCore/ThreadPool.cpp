#include "Sparkpch.h"

#include "SparkCore/HeaderFiles/ThreadPool.h"

namespace SpriteSpark {

    ThreadPool::ThreadPool(size_t numThreads) : stop(false), activeTasks(0) {
        resize(numThreads);
    }

    ThreadPool::~ThreadPool() {
        stopAll();
    }

    void ThreadPool::waitForCompletion() {
        std::unique_lock<std::mutex> lock(queueMutex);
        completionCondition.wait(lock, [this] { return tasks.empty() && activeTasks == 0; });
    }

    bool ThreadPool::waitForCompletionFor(size_t milliseconds) {
        std::unique_lock<std::mutex> lock(queueMutex);
        return completionCondition.wait_for(lock, std::chrono::milliseconds(milliseconds), [this] { return tasks.empty() && activeTasks == 0; });
    }

    void ThreadPool::resize(size_t numThreads) {
        stopAll();
        stop = false;
        if (numThreads <= 1 || numThreads > std::thread::hardware_concurrency()) {
            numThreads = std::thread::hardware_concurrency();
        }

        std::cout << "ThreadPool: Resized to: " << numThreads << std::endl;
        workers.reserve(numThreads);
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(&ThreadPool::workerThread, this);
        }
    }

    void ThreadPool::stopAll() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
        workers.clear();
    }

    void ThreadPool::workerThread() {
        while (true) {
            std::function<void()> task;
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                condition.wait(lock, [this] { return stop || !tasks.empty(); });
                if (stop && tasks.empty()) return;
                if (!tasks.empty()) {
                    task = std::move(tasks.front());
                    tasks.pop();
                }
            }
            if (task) {
                try {
                    task();
                }
                catch (const std::exception& e) {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    std::cerr << "Worker exception: " << e.what() << std::endl;
                }
                catch (...) {
                    std::lock_guard<std::mutex> lock(queueMutex);
                    std::cerr << "Worker exception: unknown exception" << std::endl;
                }
            }
        }
    }
}