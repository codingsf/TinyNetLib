#ifndef TIMER_THREAD_H_
#define TIMER_THREAD_H_

#include <boost/noncopyable.hpp>
#include <functional>
#include "Timer.h"
#include "Thread.h"

namespace Tiny
{
class TimerThread : boost::noncopyable
{
public:
    typedef std::function<void ()> Callback;

    TimerThread(double value,
                double interval,
                Callback cb);
    void start();
    void stop();


private:
    Timer timer_;
    Thread thread_;
};
}

#endif //TIMER_THREAD_H_