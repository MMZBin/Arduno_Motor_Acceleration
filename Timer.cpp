#include "Timer.h"

Timer::Timer(TimeFunc timer) : interval_(0), startTime_(0), isRunning_(false), timer_(timer) {  }

void Timer::setInterval(uint32_t interval) { if (!isRunning_) { interval_ = interval; } }

void Timer::start() {
    if (isRunning_) { return; }
    isRunning_ = true;
    startTime_ = timer_();
}

void Timer::start(uint32_t interval) {
    setInterval(interval);
    start();
}

void Timer::cancel() { isRunning_ = false; }

bool Timer::isRunning() const { return isRunning_; }

bool Timer::hasOccurred() {
    if (!isRunning_) { return false; }
    bool hasOccurred = timer_() - startTime_ >= interval_;
    if (hasOccurred) { isRunning_ = false; }
    return hasOccurred;
}
