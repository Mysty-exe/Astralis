#pragma once
#include <ctime>

class Timer
{
public:
    Timer();

    void start();
    void stop();
    void pause();
    void unpause();

    double getTicks() const; // Returns elapsed time in milliseconds
    bool isStarted() const;
    bool isPaused() const;

private:
    std::clock_t mStartTime; // Start time in clock ticks
    double mPausedDuration;  // Total paused duration in milliseconds
    bool mStarted;           // Is the timer running?
    bool mPaused;            // Is the timer paused?
};