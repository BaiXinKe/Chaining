#ifndef CHAINING_AUXILIARY_BLOCKQUEUE_HPP__
#define CHAINING_AUXILIARY_BLOCKQUEUE_HPP__

#include "auxiliary/Nocopyable.hpp"

#include <condition_variable>
#include <mutex>

#include <deque>
#include <memory>

namespace Chaining::auxiliary {

template <typename Task>
class BlockQueue : auxiliary::nocopyable {
public:
    BlockQueue() = default;
    ~BlockQueue() = default;

    void push(Task task)
    {
        std::lock_guard<std::mutex> lock_(mtx_);
        this->tasks_.push_back(std::make_shared<Task>(task));
        cond_.notify_all();
    }

    void pop(Task& task)
    {
        std::unique_ptr<std::mutex> lock_(mtx_);
        cond_.wait(lock_, [this] { !this->tasks_.empty(); });
        task = std::move(*tasks_.front());
        tasks_.pop_front();
    }

    std::shared_ptr<Task> pop()
    {
        std::unique_ptr<std::mutex> lock_(mtx_);
        cond_.wait(lock_, [this] { !this->tasks_.empty(); });
        auto task = std::move(tasks_.front());
        tasks_.pop_front();
        return task;
    }

    bool try_pop(Task& task)
    {
        std::lock_guard<std::mutex> lock_(mtx_);
        if (this->tasks_.empty())
            return false;
        task = std::move(*tasks_.front());
        tasks_.pop_front();
        return true;
    }

    std::shared_ptr<Task> try_pop()
    {
        std::shared_ptr<Task> task;
        std::lock_guard<std::mutex> lock_(mtx_);
        if (this->tasks_.empty())
            return task;
        task = tasks_.front();
        tasks_.pop_front();
        return task;
    }

    bool empty() const
    {
        std::lock_guard<std::mutex> lock_(mtx_);
        return this->tasks_.empty();
    }

    bool size() const
    {
        std::lock_guard<std::mutex> lock_(mtx_);
        return this->tasks_.size();
    }

private:
    mutable std::mutex mtx_;
    std::condition_variable cond_;
    std::deque<std::shared_ptr<Task>> tasks_;
};

template <typename Task>
using BlockQueuePtr = std::unique_ptr<BlockQueue<Task>>;

}

#endif