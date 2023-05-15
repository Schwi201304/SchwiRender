#include "swpch.h"
#include "MenuLayer.h"

#include "Core/Application.h"

namespace schwi {
	Ref<MenuLayer> MenuLayer::s_Instance = nullptr;

	MenuLayer::MenuLayer() :Layer("Menu Layer")
	{
		s_Instance.reset(this);
	}

	void MenuLayer::OnAttach()
	{

	}

	void MenuLayer::OnUpdate(Timestep ts)
	{

	}

	void MenuLayer::OnImGuiRender()
	{
		bool showMainWindows = Application::Get().GetRunStatus();
		ImGui::Begin("Schwi Engine", &showMainWindows,
			ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar);
		Application::Get().SetRunStatus(showMainWindows);

		if (ImGui::BeginMenuBar())
		{
			if(ImGui::BeginMenu("File"))
			{
				if(ImGui::MenuItem("New"))
				{

				}
				if (ImGui::MenuItem("Open"))
				{

				}
			}
			if (ImGui::BeginMenu("Edit"))
			{

			}
			
			ImGui::EndMenu();
		}

		ImGui::End();

	}

	void MenuLayer::OnEvent(Event& e)
	{

	}
}