#pragma once
#include <sstream>

#include "hazel/events/event.h"


namespace Hazel
{
    class KeyEvent : public Event
    {
    public:
        [[nodiscard]] uint32_t getKeyCode() const { return m_key_code; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }

    protected:
        explicit KeyEvent(uint32_t keycode) : m_key_code(keycode) {}
        uint32_t m_key_code;
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(uint32_t keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyTypeEvent: " << m_key_code;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::KeyTyped; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "KeyTyped"; }
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(uint32_t key_code, uint32_t repeat_count) : KeyEvent(key_code), m_repeat_count(repeat_count) {}
        [[nodiscard]] uint32_t getRepeatCount() const { return m_repeat_count; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyPressedEvent: " << m_key_code << " (" << m_repeat_count << " repeats)";
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::KeyPressed; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "KeyPressed"; }

    private:
        uint32_t m_repeat_count;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        explicit KeyReleasedEvent(uint32_t keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyReleasedEvent: " << m_key_code;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::KeyReleased; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "KeyReleased"; }
    };

}  // namespace Hazel