#include "Clock.h"


Brokkr::Clock::Clock()
{
    //starts the clock cycle
    m_startTime = std::chrono::high_resolution_clock::now();
}

//Gets time difference from last start point
float Brokkr::Clock::GetElapsedTimeInSeconds() const
{
    const auto elapsedTime = std::chrono::steady_clock::now() - m_startTime;
    return std::chrono::duration_cast<std::chrono::duration<float>>(elapsedTime).count();
}

//Converts int to std::chrono::seconds
std::chrono::seconds Brokkr::Clock::AsSeconds(int amount)
{
    return std::chrono::seconds(amount);
}

//Restarts starting time mark
void Brokkr::Clock::Restart()
{
    //Restarts the clock cycle
    m_startTime = std::chrono::high_resolution_clock::now();
}