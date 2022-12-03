#pragma once
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Hazel
{
	class OpenGlContext:public GraphicsContext
	{
	public:
		OpenGlContext(GLFWwindow* windowHandle);
		virtual void Init() override;
		virtual void SwapBuffers() override;
	protected:
	private:
		GLFWwindow* m_WindowHandle;
	};
}

