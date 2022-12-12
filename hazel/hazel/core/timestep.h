#pragma once

namespace Hazel
{
    class Timestep
    {
    public:
        explicit Timestep(const float time) : m_time(time) {}
        explicit operator float() const { return m_time; }

        [[nodiscard]] float GetSeconds() const { return m_time; }
        [[nodiscard]] float GetMilliseconds() const { return m_time * 1000.0F; }

    private:
        float m_time;
    };
}  // namespace Hazel