#include "hzpch.h"
#include "OpenGLContext.h"

#include <glfw/glfw3.h>
#include <glad/glad.h>
#include <GL/GL.h>

namespace Hazel
{

    OpenGlContext::OpenGlContext(GLFWwindow* windowHandle) : m_WindowHandle(windowHandle)
    {
        HZ_CORE_ASSERT(windowHandle, "window handle is null!")
    }

    void OpenGlContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);
        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        HZ_CORE_INFO("OpenGL info:");
        HZ_CORE_INFO("Vendor:{0}", (char*)glGetString(GL_VENDOR));
        HZ_CORE_INFO("Renderer:{0}", (char*)glGetString(GL_RENDERER));
        HZ_CORE_INFO("Version:{0}", (char*)glGetString(GL_VERSION));
    }

    void OpenGlContext::SwapBuffers() { glfwSwapBuffers(m_WindowHandle); }

} // namespace Hazel