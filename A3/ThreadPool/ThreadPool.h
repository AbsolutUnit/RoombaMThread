#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class ThreadPool {
public:
    // Initialize constructor with num threads
    ThreadPool(int numThreads) : stop(false), activeThreads(0) {
        // Loop through and emplace back current Thread class
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
                // Loop until either task added or told to stop
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        condition.wait(lock, [this] { return stop || !tasks.empty(); });
                        if (stop && tasks.empty()) {
                            return;
                        }
                        ++activeThreads;
                        task = std::move(tasks.front());
                        tasks.pop();
                    }
                    // Task call
                    task();
                    {
                        // Mutex on queue for race conditions
                        std::unique_lock<std::mutex> lock(queueMutex);
                        --activeThreads;
                        if (tasks.empty() && activeThreads == 0) {
                            // Set completion condition conditional variable
                            completionCondition.notify_all();
                        }
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        {
            // Lock queue
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        // Notify all via conditional variable
        condition.notify_all();
        for (std::thread& thread : threads) {
            // Join all threads
            thread.join();
        }
    }

    template<typename Func, typename... Args>
    void enqueue(Func&& func, Args&&... args) {
        // Create task based on binding of args and function
        auto task = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        {
            // lock the queue while grabbing task
            std::unique_lock<std::mutex> lock(queueMutex);
            // Add task to tasks queue
            tasks.emplace([task] { (*task)(); });
        }
        // Notify on completion
        condition.notify_one();
    }

    void wait() {
        // Lock the queue
        std::unique_lock<std::mutex> lock(queueMutex);
        // Await completion condition
        completionCondition.wait(lock, [this]() { return (tasks.empty() && activeThreads == 0); });
    }

private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::condition_variable completionCondition;
    bool stop;
    int activeThreads;
};
