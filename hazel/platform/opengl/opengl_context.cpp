#include "platform/opengl/opengl_context.h"

#include <glad/glad.h>

#include "hazel/core/core.h"

namespace Hazel
{
    OpenGLContext::OpenGLContext(GLFWwindow *window_handle) : m_window_handle(window_handle)
    {
        HZ_CORE_ASSERT(window_handle, "window handle is null!")
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_window_handle);
        int status = gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress));
        HZ_CORE_ASSERT(status, "Failed to initialize Glad!");

        HZ_CORE_INFO("OpenGL info:");
        HZ_CORE_INFO("Vendor:{0}", (char *)glGetString(GL_VENDOR));
        HZ_CORE_INFO("Renderer:{0}", (char *)glGetString(GL_RENDERER));
        HZ_CORE_INFO("Version:{0}", (char *)glGetString(GL_VERSION));

        int version_major = 0;
        int version_minor = 0;
        glGetIntegerv(GL_MAJOR_VERSION, &version_major);
        glGetIntegerv(GL_MINOR_VERSION, &version_minor);
        HZ_CORE_ASSERT(version_major > 4 || (version_major == 4 && version_minor >= 5),
                       "Hazel requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers() { glfwSwapBuffers(m_window_handle); }

}  // namespace Hazel