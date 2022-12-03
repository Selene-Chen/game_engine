#pragma once
#include "Hazel/Window.h"
#include "Hazel/Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Hazel
{
	class WindowsWindow :public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		virtual ~WindowsWindow();

		void OnUpdate() override;

		inline unsigned int GetWidth() const override { return m_WinData.Width; }
		inline unsigned int GetHeight() const override { return m_WinData.Height; }
		inline void SetEventCallback(const EventCallbackFn& callback) override { m_WinData.EventCallback = callback; }

		void SetVSync(bool enabled) override;
		bool IsVSync() const override;

		inline virtual void* GetNativeWindow() const override { return m_WindowHandle; }
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown();

	private:
		GLFWwindow* m_WindowHandle;
		GraphicsContext* m_Context;
		struct WindowData
		{
			std::string Title;
			unsigned int Width, Height;
			bool VSync;
			EventCallbackFn EventCallback;
		};
		WindowData m_WinData;
	};
}

