
#include <iostream>
#include "Application.h"
#include "Log.h"

int main() 
{
	schwi::Log::Init();
	SW_LOG_TRACE("trace");
	SW_LOG_DEBUG("debug");
	SW_LOG_INFO("info");
	SW_LOG_WARN("warn");
	SW_LOG_ERROR("error");
	auto app = new schwi::Application();
	app->Run();
	std::cout << "Hello!";

	delete app;
	return 0;
}