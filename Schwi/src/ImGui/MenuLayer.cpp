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
				ImGui::Checkbox("Editor", &m_ShowEditWindow);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Setting"))
			{
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();
		//ShowEditWindow();
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

	void MenuLayer::ShowEditWindow()
	{
		if (ImGui::Begin("Editor", &m_ShowEditWindow))
		{
			//if (ImGui::BeginTabBar("tabs", ImGuiTabBarFlags_None))
			//{
			//	auto scene = SceneLayer::GetInstance()->GetScene();
			//	if (ImGui::BeginTabItem("Gizmo"))
			//	{
			//	}
			//	if (ImGui::BeginTabItem("Light"))
			//	{
			//		SW_DEBUG("LIght");
			//		ImGui::SeparatorText("Point Light");
			//
			//		ImGui::EndTabItem();
			//	}
			//	if (ImGui::BeginTabItem("Model"))
			//	{
			//		ImGui::EndTabItem();
			//	}
			//	ImGui::EndTabBar();
			//}
			//auto camera = SceneLayer::GetInstance()->GetScene()->m_CameraController->GetCamera();
			//auto view = camera->GetViewMatrix();
			//auto proj = camera->GetProjectionMatrix();
			//auto transform = SceneLayer::GetInstance()->GetScene()->GetModel(0)->GetTransform();
			//
			//EditTransform(glm::value_ptr(view), glm::value_ptr(proj), glm::value_ptr(transform), true);
			ImGui::End();
		}
	}

	void MenuLayer::OnEvent(Event& e)
	{

	}

	void MenuLayer::EditTransform(float* cameraView, float* cameraProjection, float* matrix, bool editTransformDecomposition)
	{
		static ImGuizmo::MODE mCurrentGizmoMode(ImGuizmo::LOCAL);
		static bool useSnap = false;
		static float snap[3] = { 1.f, 1.f, 1.f };
		static float bounds[] = { -0.5f, -0.5f, -0.5f, 0.5f, 0.5f, 0.5f };
		static float boundsSnap[] = { 0.1f, 0.1f, 0.1f };
		static bool boundSizing = false;
		static bool boundSizingSnap = false;

		if (editTransformDecomposition)
		{
			if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_T))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_R))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_F)) // r Key
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			if (ImGui::RadioButton("Translate", mCurrentGizmoOperation == ImGuizmo::TRANSLATE))
				mCurrentGizmoOperation = ImGuizmo::TRANSLATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Rotate", mCurrentGizmoOperation == ImGuizmo::ROTATE))
				mCurrentGizmoOperation = ImGuizmo::ROTATE;
			ImGui::SameLine();
			if (ImGui::RadioButton("Scale", mCurrentGizmoOperation == ImGuizmo::SCALE))
				mCurrentGizmoOperation = ImGuizmo::SCALE;
			float matrixTranslation[3], matrixRotation[3], matrixScale[3];
			ImGuizmo::DecomposeMatrixToComponents(matrix, matrixTranslation, matrixRotation, matrixScale);
			ImGui::InputFloat3("Tr", matrixTranslation);
			ImGui::InputFloat3("Rt", matrixRotation);
			ImGui::InputFloat3("Sc", matrixScale);
			ImGuizmo::RecomposeMatrixFromComponents(matrixTranslation, matrixRotation, matrixScale, matrix);

			if (mCurrentGizmoOperation != ImGuizmo::SCALE)
			{
				if (ImGui::RadioButton("Local", mCurrentGizmoMode == ImGuizmo::LOCAL))
					mCurrentGizmoMode = ImGuizmo::LOCAL;
				ImGui::SameLine();
				if (ImGui::RadioButton("World", mCurrentGizmoMode == ImGuizmo::WORLD))
					mCurrentGizmoMode = ImGuizmo::WORLD;
			}
			if (ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_E))
				useSnap = !useSnap;
			ImGui::Checkbox("", &useSnap);
			ImGui::SameLine();

			switch (mCurrentGizmoOperation)
			{
			case ImGuizmo::TRANSLATE:
				ImGui::InputFloat3("Snap", &snap[0]);
				break;
			case ImGuizmo::ROTATE:
				ImGui::InputFloat("Angle Snap", &snap[0]);
				break;
			case ImGuizmo::SCALE:
				ImGui::InputFloat("Scale Snap", &snap[0]);
				break;
			}
			ImGui::Checkbox("Bound Sizing", &boundSizing);
			if (boundSizing)
			{
				ImGui::PushID(3);
				ImGui::Checkbox("", &boundSizingSnap);
				ImGui::SameLine();
				ImGui::InputFloat3("Snap", boundsSnap);
				ImGui::PopID();
			}
		}

		auto viewSize = SceneLayer::GetInstance()->GetViewportSize();

		//ImGui::SetNextWindowSize(ImVec2(800, 400));
		//ImGui::SetNextWindowPos(ImVec2(400, 20));
		ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor(0.35f, 0.3f, 0.3f));
		ImGui::Begin("Scene", nullptr, ImGuiWindowFlags_NoMove);

		auto scene = SceneLayer::GetInstance()->GetScene();
		//ImGuizmo::DrawCubes(cameraView, cameraProjection, &objectMatrix[0][0], gizmoCount);
		ImGuizmo::Manipulate(cameraView, cameraProjection,
			mCurrentGizmoOperation, mCurrentGizmoMode,
			matrix, NULL,
			useSnap ? &snap[0] : NULL,
			boundSizing ? bounds : NULL,
			boundSizingSnap ? boundsSnap : NULL);
		float camDistance = glm::length(scene->m_CameraController->GetCamera()->GetPosition());
		ImGuizmo::ViewManipulate(cameraView, camDistance,
			ImVec2(ImGui::GetWindowPos().x + viewSize.x - 128, ImGui::GetWindowPos().y),
			ImVec2(128, 128), 0x10101010);

		ImGui::End();
		ImGui::PopStyleColor(1);

	}
}