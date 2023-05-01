#include "swpch.h"
#include "Schwi.h"

class ExampleLayer : public schwi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		SW_INFO("ExampleLayer::Update,Name:{}",m_Name);
	}

	void OnEvent(schwi::Event& event) override
	{
		SW_TRACE("{0}", event);
	}

};

class Sandbox : public schwi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};
schwi::Application* schwi::CreateApplication()
{
	return new Sandbox();
}
int main() 
{
	schwi::Log::Init();
	std::cout << "Hello!"<<std::endl;
	auto app = schwi::CreateApplication();
	app->Run();

	delete app;
	return 0;
}