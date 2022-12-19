

#include "hazel/core/entry_point.h"

#include "Sandbox2D.h"

class Sandbox : public Hazel::Application
{
public:
    Sandbox() { PushLayer(Hazel::CreateRef<Sandbox2D>()); }
    ~Sandbox(){};
};
Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }