#include "ThreadPool.h"
#include <assert.h>

using namespace Tiny;

ThreadPool::ThreadPool(size_t queueSize, size_t threadsNum)
: empty_(mutex_),
  full_(mutex_),
  queueSize_(queueSize),
  threadsNum_(threadsNum),
  isStarted_(false)
{

}

ThreadPool::~ThreadPool()
{
    if(isStarted_)
        stop();
}

void ThreadPool::start()
{
    isStarted_ = true;
    //std::vector<std::unique<Thread> >
    for(size_t ix = 0; ix != threadsNum_; ++ix)
    {
        threads_.push_back(
            std::unique_ptr<Thread>(
                new Thread(
                    std::bind(&ThreadPool::runInThread, this))));
    }
    for(size_t ix = 0; ix != threadsNum_; ++ix)
    {
        threads_[ix]->start();
    }

}

void ThreadPool::stop()
{
    if(isStarted_ == false)
        return;

    {
        MutexLockGuard lock(mutex_);
        isStarted_ = false;
        //清空任务
        while(!queue_.empty()) 
            queue_.pop();
    }
    full_.notifyAll(); //激活所有的线程
    empty_.notifyAll();
    
    for(size_t ix = 0; ix != threadsNum_; ++ix)
    {
        threads_[ix]->join();
    }
    threads_.clear();
}

void ThreadPool::addTask(Task task)
{
    if(!isStarted_)
        return;

    MutexLockGuard lock(mutex_);
    while(queue_.size() >= queueSize_ && isStarted_)
        empty_.wait();

    if(!isStarted_)
        return;

    queue_.push(std::move(task));
    full_.notify();
}


ThreadPool::Task ThreadPool::getTask()
{
    MutexLockGuard lock(mutex_);
    while(queue_.empty() && isStarted_)
        full_.wait();

    if(!isStarted_) //线程池关闭
        return Task(); //空任务

    assert(!queue_.empty());
    Task task = queue_.front();
    queue_.pop();
    empty_.notify();
    return task;
}

void ThreadPool::runInThread()
{
    while(isStarted_)
    {
        Task task(getTask());
        if(task)
            task();
    }
}
