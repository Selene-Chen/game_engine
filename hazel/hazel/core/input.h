#pragma once
#include "hazel/core/core.h"
#include "hazel/core/key_code.h"
#include "hazel/core/mouse_code.h"

namespace Hazel
{
    // 单例结构
    class Input
    {
    protected:
        Input() = default;

    public:
        virtual ~Input()               = default;
        Input(const Input&)            = delete;  // 删除拷贝函数
        Input& operator=(const Input&) = delete;  // 删除赋值拷贝操作

        static bool IsKeyPressed(KeyCode keycode) { return m_instance->IsKeyPressedImpl(keycode); }
        static bool IsMouseButtonPressed(MouseCode button) { return m_instance->IsMouseButtonPressedImpl(button); }
        static std::pair<float, float> GetMousePosition() { return m_instance->GetMousePositionImpl(); }
        static float GetMouseX() { return m_instance->GetMouseXImpl(); }
        static float GetMouseY() { return m_instance->GetMouseYImpl(); }

    protected:
        virtual bool IsKeyPressedImpl(KeyCode keycode)          = 0;
        virtual bool IsMouseButtonPressedImpl(MouseCode button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl()  = 0;
        virtual float GetMouseXImpl()                           = 0;
        virtual float GetMouseYImpl()                           = 0;

    private:
        static Scope<Input> m_instance;
    };
}  // namespace Hazel