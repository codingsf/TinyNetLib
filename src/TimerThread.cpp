#include "TimerThread.h"

namespace Tiny
{
TimerThread::TimerThread(double value,
                         double interval,
                         Callback cb)
: timer_(value, interval, std::move(cb)),
  thread_(std::bind(&Timer::start, &timer_))
{

}
void TimerThread::start()
{
    thread_.start();
}

void TimerThread::stop() //提供给别的线程使用
{
    timer_.stop(); 
    thread_.join();
    //线程自动退出
}
}
