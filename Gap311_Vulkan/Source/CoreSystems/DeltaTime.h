#pragma once

namespace Brokkr
{
    class DeltaTime final
    {
        double m_timeStep = 0.;
    public:
        void SetTime(const double time) { m_timeStep = time; }

        [[nodiscard]] double GetSeconds()const { return m_timeStep; }

        [[nodiscard]] double GetMilliseconds()const { return m_timeStep * 1000.; }

        // for cool thing like casting to a float by default timeStep is seconds
        explicit operator double() const { return m_timeStep; }

    };
}