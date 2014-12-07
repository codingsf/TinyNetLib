#ifndef TIMER_H
#define TIMER_H


#include <boost/noncopyable.hpp>
#include <sys/timerfd.h>
#include <functional>

namespace Tiny
{
class Timer : boost::noncopyable
{
public:
    typedef std::function<void ()> TimerCallback;

    Timer(double val, 
          double interval, 
          TimerCallback cb);
    ~Timer();

    void start();
    void stop();

private:
    int timerfd_;
    double val_;
    double interval_;
    TimerCallback callback_;
    bool isStarted_;
};
}

#endif //TIMER_H