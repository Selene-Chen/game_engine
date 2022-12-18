#pragma once
#include <sstream>

#include "hazel/core/key_code.h"
#include "hazel/events/event.h"

namespace Hazel
{
    class KeyEvent : public Event
    {
    public:
        [[nodiscard]] KeyCode getKeyCode() const { return m_keycode; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryKeyboard | EventCategoryInput; }

    protected:
        explicit KeyEvent(KeyCode keycode) : m_keycode(keycode) {}
        KeyCode m_keycode;
    };

    class KeyTypedEvent : public KeyEvent
    {
    public:
        explicit KeyTypedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyTypeEvent: " << m_keycode;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::KeyTyped; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "KeyTyped"; }
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(KeyCode keycode, uint32_t repeat_count) : KeyEvent(keycode), m_repeat_count(repeat_count) {}
        [[nodiscard]] uint32_t getRepeatCount() const { return m_repeat_count; }

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyPressedEvent: " << m_keycode << " (" << m_repeat_count << " repeats)";
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
        explicit KeyReleasedEvent(KeyCode keycode) : KeyEvent(keycode) {}

        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "KeyReleasedEvent: " << m_keycode;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::KeyReleased; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "KeyReleased"; }
    };

}  // namespace Hazel