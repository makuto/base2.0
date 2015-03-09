#ifndef TIMER_CPP
#define TIMER_CPP
#include "timer.hpp"

void timer::start()
{
    myClock.restart();
    //For some reason the first time is always bad, so we will waste it here
    getTime();
}
float timer::getTime()
{
    return myClock.getElapsedTime().asSeconds();
}
#endif
