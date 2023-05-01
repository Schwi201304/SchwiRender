#include "swpch.h"
#include "Schwi.h"


int main() 
{
	schwi::Log::Init();
	std::cout << "Hello!"<<std::endl;
	auto app = new schwi::Application();
	app->Run();

	delete app;
	return 0;
}