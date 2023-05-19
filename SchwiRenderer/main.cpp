#include <Schwi.h>
#include <Core/EntryPoint.h>

class APP : public schwi::Application
{
public:
	APP()
	{
	}

	~APP()
	{
	}

};

schwi::Application* schwi::CreateApplication()
{
	return new APP();
}