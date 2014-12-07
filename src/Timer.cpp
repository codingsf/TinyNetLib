#include "Timer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/timerfd.h>
#include <stdint.h>
#include <poll.h>
#define ERR_EXIT(m) \
    do { \
        perror(m);\
        exit(EXIT_FAILURE);\
    }while(0)

namespace  //static
{

const long kNsecondsOfPerSecond = 1000 * 1000 * 1000;

int createTimerfd()
{
    int timerfd = ::timerfd_create(CLOCK_REALTIME, 0);
    if(timerfd == -1)
        ERR_EXIT("timerfd_create"); 
    return timerfd;
}

void setTimerfd(int timerfd, double value, double interval)
{
    struct itimerspec val;
    memset(&val, 0, sizeof val);
    val.it_value.tv_sec = (time_t)value;
    val.it_value.tv_nsec = (value - (time_t)value) * kNsecondsOfPerSecond;
    val.it_interval.tv_sec = (time_t)interval;
    val.it_interval.tv_nsec = (interval - (time_t)interval) * kNsecondsOfPerSecond;

    if(::timerfd_settime(timerfd, 0, &val, NULL) == -1)
        ERR_EXIT("timerfd_settime");
}

void stopTimerfd(int timerfd)
{
    setTimerfd(timerfd, 0, 0);
}

void readTimerfd(int timerfd)
{
    uint64_t howmany;
    if(::read(timerfd, &howmany, sizeof howmany) != sizeof(howmany))
    {
        ERR_EXIT("read timerfd error");
    }
}

}


namespace Tiny
{
Timer::Timer(double val, 
      double interval, 
      TimerCallback cb)
: timerfd_(createTimerfd()),
  val_(val),
  interval_(interval),
  callback_(std::move(cb)),
  isStarted_(false)
{

}

Timer::~Timer()
{
    if(isStarted_)
    {
        stop();
        ::close(timerfd_);
    }
}


void Timer::start()
{
    setTimerfd(timerfd_, val_, interval_);
    isStarted_ = true;

    struct pollfd pfd;
    pfd.fd = timerfd_;
    pfd.events = POLLIN; //监听输入事件

    int ret;
    while(isStarted_)
    {
        ret = ::poll(&pfd, 1, 5000);
        if(ret == -1)
        {
            if(errno == EINTR)
                continue;
            ERR_EXIT("poll");
        }
        else if(ret == 0)
        {
            printf("timeout\n"); //超时
            continue;
        }

        if(pfd.revents == POLLIN) //此fd是否监听的read事件
        {
            readTimerfd(timerfd_);
            //foo();
            callback_();
        }
            
    }
}

void Timer::stop()
{
    isStarted_ = false;
    stopTimerfd(timerfd_);
}
}
