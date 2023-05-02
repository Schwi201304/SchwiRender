#include "swpch.h"
#include "Schwi.h"
#include <imgui.h>

class ExampleLayer : public schwi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		//if (schwi::Input::IsKeyPressed(schwi::Key::Space))
		//	SW_TRACE("Space key is pressed (poll)!");
	}

	void OnEvent(schwi::Event& event) override
	{
		//if (event.GetEventType() == schwi::EventType::KeyPressed)
		//{
		//	schwi::KeyPressedEvent& e = (schwi::KeyPressedEvent&)event;
		//	if (e.GetKeyCode() == schwi::Key::Tab)
		//		SW_TRACE("Tab key is pressed (event)!");
		//	SW_TRACE("{0}", (char)e.GetKeyCode());
		//}
	}

	virtual void OnImGuiRender() override
	{
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::End();
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
	std::cout << "Hello!" << std::endl;
	auto app = schwi::CreateApplication();
	app->Run();

	delete app;
	return 0;
}