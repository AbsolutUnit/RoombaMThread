#include <thread>
#include <mutex>
#include <future>
#include <condition_variable>
#include <queue>
#include <functional>
#include <vector>

class ThreadPool {
public:
    ThreadPool(int numThreads) : stop(false), activeThreads(0) {
        for (int i = 0; i < numThreads; ++i) {
            threads.emplace_back([this] {
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
                    task();
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        --activeThreads;
                        if (tasks.empty() && activeThreads == 0) {
                            completionCondition.notify_all();
                        }
                    }
                }
            });
        }
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for (std::thread& thread : threads) {
            thread.join();
        }
    }

    template<typename Func, typename... Args>
    void enqueue(Func&& func, Args&&... args) {
        auto task = std::make_shared<std::packaged_task<void()>>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            tasks.emplace([task] { (*task)(); });
        }
        condition.notify_one();
    }

    void wait() {
        std::unique_lock<std::mutex> lock(queueMutex);
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
