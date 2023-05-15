#pragma once

#include "Core/Core.h"
#include "Application.h"

#ifdef SW_PLATFORM_WINDOWS
extern schwi::Application* schwi::CreateApplication();

#ifdef SW_BUILD_EXE
class APP : public schwi::Application
{
public:
	APP(){}		
	~APP(){}		
};

schwi::Application* schwi::CreateApplication()
{
	return new APP();
}
#endif


int main(int argc, char** argv)
{
	schwi::Log::Init();
	std::cout << "Hello!" << std::endl;
	auto app = schwi::CreateApplication();
	app->Run();

	app = nullptr;
	return 0;
}

#endif//end SW_PLATFORM_WINDOWS