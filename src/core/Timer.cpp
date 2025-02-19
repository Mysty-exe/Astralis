#include <Timer.h>

Timer::Timer()
{
    mStartTime = 0;
    mPausedDuration = 0.0;
    mStarted = false;
    mPaused = false;
    increment = 0;
}

void Timer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTime = std::clock(); // Get current clock ticks
    mPausedDuration = 0.0;
}

void Timer::stop()
{
    mStarted = false;
    mPaused = false;
    mPausedDuration = 0.0;
}

void Timer::pause()
{
    if (mStarted && !mPaused)
    {
        mPaused = true;
        // Accumulate paused duration
        mPausedDuration += static_cast<double>(std::clock() - mStartTime) / CLOCKS_PER_SEC * 1000.0;
    }
}

void Timer::unpause()
{
    if (mStarted && mPaused)
    {
        mPaused = false;
        // Reset the start time to now
        mStartTime = std::clock();
    }
}

double Timer::getTicks() const
{
    if (mStarted)
    {
        if (mPaused)
        {
            // Return the total time elapsed before pause
            return increment + mPausedDuration;
        }
        else
        {
            // Return the total time elapsed, including unpaused time
            return increment + mPausedDuration + static_cast<double>(std::clock() - mStartTime) / CLOCKS_PER_SEC * 1000.0;
        }
    }
    return 0.0; // Timer is not running
}

bool Timer::isStarted() const
{
    return mStarted;
}

bool Timer::isPaused() const
{
    return mPaused;
}