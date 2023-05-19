#include "swpch.h"
#include "MenuLayer.h"

#include <ImGuizmo.h>

#include "Core/Application.h"
#include "Core/Util.h"
#include "Scene/SceneLayer.h"
#include "Scene/SceneSerializer.h"
#include "Scene/CameraController.h"
#include "Renderer/Camera.h"

namespace schwi {
	Ref<MenuLayer> MenuLayer::s_Instance = nullptr;
	static ImGuizmo::OPERATION mCurrentGizmoOperation(ImGuizmo::TRANSLATE);

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
		if (!showMainWindows)
		{
			Application::Get().Close();
		}

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New","Ctral+N"))
				{
					SceneLayer::GetInstance()->NewScene();
				}
				if (ImGui::MenuItem("Open","Ctrl+O"))
				{
					SceneLayer::GetInstance()->OpenScene();
				}
				if (ImGui::MenuItem("Save","Ctrl+S"))
				{
					SceneLayer::GetInstance()->SaveScene();
				}
				if (ImGui::MenuItem("Exit", "Alt+F4"))
				{
					Application::Get().Close();
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Setting"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
		ShowStatusWindow();
	}

	void MenuLayer::ShowStatusWindow()
	{
		if (ImGui::Begin("Status"))
		{
			auto& io = ImGui::GetIO();
			float frame_rate = io.Framerate;
			ImGui::Text("Vertices:");
			ImGui::Text("Triangles:");
			ImGui::Text("Frame Rate:%.1f,time per frame:%.3f ms", frame_rate, 1000.0f / frame_rate);
			ImGui::Text("Mouse Pos:%d,%d", (int)io.MousePos.x, (int)io.MousePos.y);
			for (int i = 0; i < IM_ARRAYSIZE(io.MouseDown); i++)
				if (ImGui::IsMouseDown(i))
				{
					ImGui::SameLine();
					ImGui::Text("b%d (%.02f secs)", i, io.MouseDownDuration[i]);
				}
			ImGui::Text("Keys down:");
			for (ImGuiKey key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_END; key = (ImGuiKey)(key + 1))
			{
				if (!ImGui::IsKeyDown(key))
					continue;
				ImGui::SameLine();
				ImGui::Text( "\"%s\" " , ImGui::GetKeyName(key));
			}
		}
	}

	void MenuLayer::OnEvent(Event& e)
	{

	}
}