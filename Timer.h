//
// Created by Edzia on 2017-11-06.
//

#ifndef PEA_1_TIMER_H
#define PEA_1_TIMER_H

#include <windows.h>

class Timer {
private:
    LARGE_INTEGER time_start;
    LARGE_INTEGER time_end;

    LARGE_INTEGER getTime();

protected:
    enum State_t {
        TIMER_NONE,
        TIMER_STARTED,
        TIMER_DONE
    };

protected:
    State_t time_state;

public:
    Timer();
    ~Timer();

    void start();
    void stop();
    double get();
};


#endif //PEA_1_TIMER_H
