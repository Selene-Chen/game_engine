#pragma once
#include <sstream>

#include "hazel/events/event.h"

namespace Hazel
{
    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(const uint32_t width, const uint32_t height) : m_width(width), m_height(height) {}

        [[nodiscard]] uint32_t GetWidth() const { return m_width; }
        [[nodiscard]] uint32_t GetHeight() const { return m_height; }
        [[nodiscard]] std::string ToString() const override
        {
            std::stringstream string_stream;
            string_stream << "WindowResizeEvent: " << m_width << ", " << m_height;
            return string_stream.str();
        }

        static EventType GetStaticType() { return EventType ::WindowResize; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "WindowResize"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryApplication; }

    private:
        uint32_t m_width, m_height;
    };

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() = default;

        static EventType GetStaticType() { return EventType ::WindowClose; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "WindowClose"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryApplication; }
    };
    class AppTickEvent : public Event
    {
    public:
        AppTickEvent() = default;

        static EventType GetStaticType() { return EventType ::AppTick; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "AppTick"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryApplication; }
    };
    class AppUpdateEvent : public Event
    {
    public:
        AppUpdateEvent() = default;

        static EventType GetStaticType() { return EventType ::AppUpdate; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "AppUpdate"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryApplication; }
    };
    class AppRenderEvent : public Event
    {
    public:
        AppRenderEvent() = default;

        static EventType GetStaticType() { return EventType ::AppRender; }
        [[nodiscard]] EventType GetEventType() const override { return GetStaticType(); }
        [[nodiscard]] const char* GetName() const override { return "AppRender"; }
        [[nodiscard]] int GetCategoryFlags() const override { return EventCategoryApplication; }
    };
}  // namespace Hazel