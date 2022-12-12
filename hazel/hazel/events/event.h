#pragma once

#include "hazel/core/core.h"

namespace Hazel
{
    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        AppTick,
        AppUpdate,
        AppRender,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };
    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput = BIT(1),
        EventCategoryKeyboard = BIT(2),
        EventCategoryMouse = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

    class Event
    {
        friend class EventDispatcher;

    public:
        virtual ~Event() = default;

        [[nodiscard]] virtual EventType GetEventType() const = 0;
        [[nodiscard]] virtual int GetCategoryFlags() const = 0;
        [[nodiscard]] virtual const char* GetName() const = 0;

        [[nodiscard]] virtual std::string ToString() const { return GetName(); }

        [[nodiscard]] bool IsInCategory(const EventCategory category) const
        {
            return (GetCategoryFlags() & category) != 0;
        }
        [[nodiscard]] bool IsHandled() const { return m_handled; }

    private:
        bool m_handled = false;
    };
    // 事件调度
    // 如果事件Type和事件处理函数Type相同，则执行事件处理函数
    // 返回true调度成功
    class EventDispatcher
    {
    public:
        explicit EventDispatcher(Event& event) : m_event(event) {}
        template <typename T, typename F>
        bool dispatch(const F& func)
        {
            if (m_event.GetEventType() == T::GetStaticType())
            {
                m_event.m_handled = func(static_cast<T&>(m_event));
                return true;
            }
            return false;
        }

    private:
        Event& m_event;
    };

    inline std::ostream& operator<<(std::ostream& ostream, const Event& event) { return ostream << event.ToString(); }
}  // namespace Hazel