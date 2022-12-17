
#include "platform/windows/windows_input.h"

#include <GLFW/glfw3.h>

#include "hazel/core/application.h"
#include "hazel/core/core.h"

namespace Hazel
{
    Scope<Input> Input::m_instance = CreateScope<WindowsInput>();

    bool WindowsInput::IsKeyPressedImpl(int keycode)
    {
        auto *window = static_cast<GLFWwindow *>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }

    bool WindowsInput::IsMouseButtonPressedImpl(int button)
    {
        auto *window = static_cast<GLFWwindow *>(Application::GetInstance().GetWindow().GetNativeWindow());
        auto state = glfwGetKey(window, button);
        return state == GLFW_PRESS;
    }

    std::pair<float, float> WindowsInput::GetMousePositionImpl()
    {
        auto *window = static_cast<GLFWwindow *>(Application::GetInstance().GetWindow().GetNativeWindow());
        double xpos = NAN;
        double ypos = NAN;
        glfwGetCursorPos(window, &xpos, &ypos);
        return {static_cast<float>(xpos), static_cast<float>(ypos)};
    }

    float WindowsInput::GetMouseXImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return x;
    }

    float WindowsInput::GetMouseYImpl()
    {
        auto [x, y] = GetMousePositionImpl();
        return y;
    }

}  // namespace Hazel