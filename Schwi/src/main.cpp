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
		static bool show = true;
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		if (ImGui::Button("Button"))
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter); 
		if (ImGui::Button("Show Example"))
			show=!show;
		ImGui::ColorEdit4("clear color", (float*)&schwi::s_ClearColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();

		if (show) {
			ImGui::Begin("Example", &show);
			ImGui::Text("Hello from another window!");
			ImGui::End();
		}
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