//
// Created by amir sarusi on 24/12/2021.
//
#pragma once
#ifndef CONSUMER_THREADPOOL_H
#define CONSUMER_THREADPOOL_H

#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <cassert>



class threadpool {
private:
    std::queue<std::function<void()>> m_function_queue;
    std::mutex m_lock;
    std::condition_variable m_data_condition;
    std::atomic<bool> m_accept_functions;

public:

    threadpool();
    ~threadpool();
    void push(std::function<void()> func);
    void done();
    void infinite_loop_func();
};


#endif //CONSUMER_THREADPOOL_H
