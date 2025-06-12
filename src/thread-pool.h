#ifndef THREAD_POOL_H
#define THREAD_POOL_H
#include <thread>
#include <vector>
#include <queue>
#include <functional>
#include <mutex>

// ThreadPool class for managing a pool of worker threads
class ThreadPool {
private:
    std::vector<std::thread> workers; // Stores worker threads that execute tasks concurrently

public:
    ThreadPool(const ThreadPool&) = delete; // Disable copy constructor
    ThreadPool& operator=(const ThreadPool&) = delete; // Disable assignment operator
    ThreadPool(ThreadPool&&) = delete; // Disable move constructor
    ThreadPool& operator=(ThreadPool&&) = delete; // Disable move assignment operator

    ThreadPool(size_t numThreads, std::function<void()> task) {
        std::function<void()> downloadTask = [&task]() {
            task();
        };
        // create a pool of worker threads
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back(std::thread(downloadTask));
        }
    }
    void waitForCompletion() {
        // wait for all worker threads to finish
        for (auto& worker : workers) {
            if (worker.joinable()) {
                worker.join();
            }
        }
    }
    ~ThreadPool() {
        // Destructor to join all threads
        waitForCompletion();
    }
};

#endif // THREAD_POOL_H