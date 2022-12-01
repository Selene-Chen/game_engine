#include <Hazel.h>

class Sanbox : public Hazel::Applicaiton
{
public:
	Sanbox() {}
	~Sanbox() {}
};

Hazel::Applicaiton* Hazel::CreateApplication()
{
	return new Sanbox();
}