#include <Timer.h>

Timer::Timer()
{
    mStartTime = 0;
    mPausedDuration = 0.0;
    mStarted = false;
    mPaused = false;
    increment = 0;
}

void Timer::reset()
{
    mStarted = false;
    mPaused = false;
    mStartTime = 0;
    mPausedDuration = 0.0;
}

void Timer::start()
{
    mStarted = true;
    mPaused = false;
    mStartTime = std::clock();
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
        mPausedDuration += static_cast<double>(std::clock() - mStartTime) / CLOCKS_PER_SEC * 1000.0;
    }
}

void Timer::unpause()
{
    if (mStarted && mPaused)
    {
        mPaused = false;
        mStartTime = std::clock();
    }
}

double Timer::getTicks() const
{
    if (mStarted)
    {
        if (mPaused)
        {
            return increment + mPausedDuration;
        }
        else
        {
            return increment + mPausedDuration + static_cast<double>(std::clock() - mStartTime) / CLOCKS_PER_SEC * 1000.0;
        }
    }
    return 0.0;
}

bool Timer::isStarted() const
{
    return mStarted;
}

bool Timer::isPaused() const
{
    return mPaused;
}