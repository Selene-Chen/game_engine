
#include "platform/windows/WindowsWindow.h"

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "hazel/core/core.h"
#include "hazel/events/application_event.h"
#include "hazel/events/key_event.h"
#include "hazel/events/mouse_event.h"
#include "platform/opengl/opengl_context.h"

namespace Hazel
{
    static uint8_t s_GLFWWindowCount = 0;  // 窗口计数
    // 子类用父类创建
    Scope<Window> Window::Create(const WindowProps &props) { return CreateScope<WindowsWindow>(props); }

    WindowsWindow::WindowsWindow(const WindowProps &props) : m_window(nullptr) { WindowsWindow::Init(props); }

    WindowsWindow::~WindowsWindow() { WindowsWindow::Shutdown(); }

    static void GLFWErrorCallback(int error, const char *description)
    {
        HZ_CORE_ERROR("GLFW Error ({0}): {1}", error, description);
    }

    void WindowsWindow::Init(const WindowProps &props)
    {
        m_win_data.Title = props.Title;
        m_win_data.Width = props.Width;
        m_win_data.Height = props.Height;
        HZ_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (s_GLFWWindowCount == 0)
        {
            HZ_CORE_INFO("Initializing GLFW");
            const int success = glfwInit();
            HZ_CORE_ASSERT(success, "Could not intialize GLFW!");
            if (success != 0)
            {
                glfwSetErrorCallback(GLFWErrorCallback);
            }
        }
        m_window = glfwCreateWindow(static_cast<int>(props.Width), static_cast<int>(props.Height),
                                    m_win_data.Title.c_str(), nullptr, nullptr);
        ++s_GLFWWindowCount;  // GLFW窗口数量+1

        m_context = CreateScope<OpenGLContext>(m_window);
        m_context->Init();

        glfwSetWindowUserPointer(m_window, &m_win_data);
        SetVSync(true);

        // 设置 GLFW 回调函数
        glfwSetWindowSizeCallback(m_window, [](GLFWwindow *window, const int width, const int height) {
            WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            data.Width = width;
            data.Height = height;
            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });
        glfwSetWindowCloseCallback(m_window, [](GLFWwindow *window) {
            const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            WindowCloseEvent event;
            data.EventCallback(event);
        });
        glfwSetKeyCallback(m_window,
                           [](GLFWwindow *window, const int key, int /*scancode*/, const int action, int /*mods*/) {
                               const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
                               switch (action)
                               {
                                   case GLFW_PRESS:
                                       {
                                           KeyPressedEvent event(key, 0);
                                           data.EventCallback(event);
                                           break;
                                       }
                                   case GLFW_RELEASE:
                                       {
                                           KeyReleasedEvent event(key);
                                           data.EventCallback(event);
                                           break;
                                       }
                                   case GLFW_REPEAT:
                                       {
                                           KeyPressedEvent event(key, 1);
                                           data.EventCallback(event);
                                           break;
                                       }
                                   default:;
                               }
                           });
        glfwSetMouseButtonCallback(m_window, [](GLFWwindow *window, const int button, const int action, int /*mods*/) {
            const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            switch (action)
            {
                case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent event(button);
                        data.EventCallback(event);
                        break;
                    }
                default:;
            }
        });
        glfwSetScrollCallback(m_window, [](GLFWwindow *window, const double x_offset, const double y_offset) {
            const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            MouseScrolledEvent event(static_cast<float>(x_offset), static_cast<float>(y_offset));
            data.EventCallback(event);
        });
        glfwSetCursorPosCallback(m_window, [](GLFWwindow *window, const double x_pos, const double y_pos) {
            const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            MouseMovedEvent event(static_cast<float>(x_pos), static_cast<float>(y_pos));
            data.EventCallback(event);
        });
        glfwSetCharCallback(m_window, [](GLFWwindow *window, const uint32_t keycode) {
            const WindowData &data = *static_cast<WindowData *>(glfwGetWindowUserPointer(window));
            KeyTypedEvent event(keycode);
            data.EventCallback(event);
        });
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(const bool enabled)
    {
        if (enabled)
        {
            glfwSwapInterval(1);
        }
        else
        {
            glfwSwapInterval(0);
        }
        m_win_data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const { return m_win_data.VSync; }

    void WindowsWindow::Shutdown() { glfwDestroyWindow(m_window); }
}  // namespace Hazel