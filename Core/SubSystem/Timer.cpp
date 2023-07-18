#include "stdafx.h"
#include "Timer.h"

Timer::Timer(Context* const context) : ISubSystem(context)
{
}

bool Timer::Initialize()
{
    previous_time = std::chrono::high_resolution_clock::now();
    return true;
}

void Timer::Update()
{
    auto current_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> ms = current_time - previous_time;
    previous_time = current_time;

    delta_time_ms = ms.count();
}

StopWatch::StopWatch()
{
    Start();
}

void StopWatch::Start()
{
    start_time = std::chrono::high_resolution_clock::now();
}

const float StopWatch::GetElapsedTimeSec()
{
    std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;
    return static_cast<float>(ms.count() / 1000.0);
}

const float StopWatch::GetElapsedTimeMs()
{
    std::chrono::duration<double, std::milli> ms = std::chrono::high_resolution_clock::now() - start_time;
    return static_cast<float>(ms.count());
}