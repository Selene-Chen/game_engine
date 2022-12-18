#pragma once
#include <sstream>

#include "hazel/core/mouse_code.h"
#include "hazel/events/event.h"

namespace Hazel
{
    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(const float x, const float y) : m_mouse_x(x), m_mouse_y(y) {}

        [[nodiscard]] float GetX() const { return m_mouse_x; }
        [[nodiscard]] float GetY() const { return m_mouse_y; }
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "MouseMovedEvent: " << m_mouse_x << ", " << m_mouse_y;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::MouseMoved; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "MouseMoved"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    private:
        float m_mouse_x, m_mouse_y;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(const float x_offset, const float y_offset) : m_xoffset(x_offset), m_yoffset(y_offset) {}

        [[nodiscard]] float GetXOffset() const { return m_xoffset; }
        [[nodiscard]] float GetYOffset() const { return m_yoffset; }
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "MouseScrolledEvent: " << GetXOffset() << ", " << GetYOffset();
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::MouseScrolled; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "MouseScrolled"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    private:
        float m_xoffset, m_yoffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        [[nodiscard]] MouseCode getMouseButton() const { return m_button; }

        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryMouse | EventCategoryInput; }

    protected:
        explicit MouseButtonEvent(const MouseCode button) : m_button(button) {}
        MouseCode m_button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonPressedEvent(const MouseCode button) : MouseButtonEvent(button) {}
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "MouseButtonPressedEvent: " << m_button;
            return string_stream.str();
        }
        static EventType GetStaticType() { return EventType ::MouseButtonPressed; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "MouseButtonPressed"; }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        explicit MouseButtonReleasedEvent(const MouseCode button) : MouseButtonEvent(button) {}
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "MouseButtonReleasedEvent: " << m_button;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::MouseButtonReleased; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "MouseButtonReleased"; }
    };
}  // namespace Hazel