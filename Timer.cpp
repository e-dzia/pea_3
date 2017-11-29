//
// Created by Edzia on 2017-11-06.
//

#include "Timer.h"

Timer::Timer() {

}

Timer::~Timer() {

}

LARGE_INTEGER Timer::getTime() {
    LARGE_INTEGER timer;
    QueryPerformanceCounter(&timer);
    return timer;
}

void Timer::start() {
    time_start = getTime();
    time_state = TIMER_STARTED;
}

void Timer::stop() {
    if(time_state != TIMER_STARTED)
        return;

    time_end = getTime();
    time_state = TIMER_DONE;
}

double Timer::get() {
    if(time_state != TIMER_DONE)
        return -1.f;

    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency);
    return (double)(time_end.QuadPart - time_start.QuadPart)/frequency.QuadPart;
}
