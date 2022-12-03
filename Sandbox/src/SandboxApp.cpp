#include <Hazel.h>
class ExampleLayer : public Hazel::Layer
{
public:
	ExampleLayer():Layer("example"){}
	void OnUpdate() override
	{
		//HZ_INFO("exampleLayer:onupdate");
		if (Hazel::Input::IsKeyPressed(HZ_KEY_TAB))
			HZ_TRACE("tab key is pressed!(poll)");
	}
	void OnEvent(Hazel::Event& event) override
	{
		//HZ_INFO("exampleLayer:onupdate");
		//HZ_TRACE("{0}", event.ToString
		if (event.GetEventType() == Hazel::EventType::KeyPressed)
		{
			Hazel::KeyPressedEvent& e = (Hazel::KeyPressedEvent&)event;
			if (e.GetKeyCode() == HZ_KEY_TAB)
				HZ_TRACE("tab key is pressed!(event)");
			HZ_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};
class Sandbox : public Hazel::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
		PushOverlay(new Hazel::ImguiLayer());
	}
	~Sandbox() {}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sandbox();
}