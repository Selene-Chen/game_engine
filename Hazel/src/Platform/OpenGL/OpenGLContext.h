#pragma once
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Hazel
{
    class OpenGlContext : public GraphicsContext
    {
    public:
        OpenGlContext(GLFWwindow* windowHandle);
        OpenGlContext() = default;

    public:
        virtual void Init() override;
        virtual void SwapBuffers() override;

    private:
        GLFWwindow* m_WindowHandle;
    };
}  // namespace Hazel
