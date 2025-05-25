#pragma once
#include <chrono>

namespace Brokkr
{
    class Clock
    {
        //Running time stamp : Starts on construction : Restart with Restart()
        std::chrono::time_point<std::chrono::steady_clock> m_startTime;
    public:

        Clock();

        //Gets time difference from last start point
        [[nodiscard]] float GetElapsedTimeInSeconds() const;

        //Converts int to std::chrono::seconds
        static std::chrono::seconds AsSeconds(int amount);

        //Restarts starting time mark
        void Restart();
    };
}