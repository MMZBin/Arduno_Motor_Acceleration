#ifndef TIMER_H
#define TIMER_H

#include <Arduino.h>

class Timer {
public:
    using TimeFunc = uint32_t (*)();

    Timer(TimeFunc timer=millis);

    void setInterval(uint32_t interval);

    void start();
    void start(uint32_t interval);

    void cancel();

    bool isRunning() const;
    bool hasOccurred();

private:
    uint32_t interval_, startTime_;
    bool isRunning_;

    TimeFunc timer_;
};

#endif TIMER_H
