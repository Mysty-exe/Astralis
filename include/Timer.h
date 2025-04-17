#pragma once
#include <ctime>

class Timer
{
private:
    std::clock_t mStartTime;
    double mPausedDuration;
    bool mStarted;
    bool mPaused;
    double increment;

public:
    Timer();

    void reset();
    void start();
    void stop();
    void pause();
    void unpause();

    double getTicks() const;
    bool isStarted() const;
    bool isPaused() const;
};