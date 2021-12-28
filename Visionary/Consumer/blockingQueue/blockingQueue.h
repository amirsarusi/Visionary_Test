//
// Created by amir sarusi on 28/12/2021.
//
#ifndef CONSUMER_BLOCKINGQUEUE_H
#define CONSUMER_BLOCKINGQUEUE_H
#include <queue>
#include <cassert>
#include <mutex>
#include <condition_variable>
#include <thread>

template<typename T>
class blockingQueue
{
private:
    std::mutex _mutex;
    std::condition_variable _condvar;
    std::queue<T> _queue;
public:
    blockingQueue(): _mutex(),_condvar(),_queue()
    {

    }
    blockingQueue(const blockingQueue& rhs) = delete;
    blockingQueue& operator = (const blockingQueue& rhs) = delete;

    void Put(const T& task)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _queue.push(task);
        }
        _condvar.notify_all();
    }

    T Take()
    {
        std::unique_lock<std::mutex> lock(_mutex);
        _condvar.wait(lock,[this]{return !_queue.empty(); });
        assert(!_queue.empty());
        T front(std::move(_queue.front()));
        _queue.pop();

        return front;
    }

    bool empty(){
        return _queue.empty();
    }

};

#endif //CONSUMER_BLOCKINGQUEUE_H
