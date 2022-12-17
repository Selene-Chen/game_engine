#pragma once
#include <GLFW/glfw3.h>

#include "hazel/renderer/graphics_context.h"

namespace Hazel
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        explicit OpenGLContext(GLFWwindow *window_handle);

        void Init() override;
        void SwapBuffers() override;

    private:
        GLFWwindow *m_window_handle;
    };
}  // namespace Hazel