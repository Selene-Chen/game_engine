#include <Hazel.h>

class Sanbox : public Hazel::Application
{
public:
	Sanbox() {}
	~Sanbox() {}
};

Hazel::Application* Hazel::CreateApplication()
{
	return new Sanbox();
}