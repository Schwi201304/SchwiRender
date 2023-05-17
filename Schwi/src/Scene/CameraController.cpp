#include "swpch.h"
#include "CameraController.h"

#include "Core/Input.h"
#include "Core/Code.h"
#include "Core/Core.h"

#include <imgui.h>

namespace schwi {

	void CameraController::OnUpdate(Timestep ts)
	{
		if (!m_Active)
			return;
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_W))
			m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_S))
			m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_A))
			m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraMoveSpeed * ts * m_Camera->GetRightVector());
		if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_D))
			m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraMoveSpeed * ts * m_Camera->GetRightVector());

		if (ImGui::IsMouseDragging(ImGuiMouseButton_Left))
		{
			auto offset = ImGui::GetIO().MouseDelta;
			m_Camera->SetYaw(m_Camera->GetYaw() + m_CameraRotationSpeed * offset.x);
			m_Camera->SetPitch(m_Camera->GetPitch() - m_CameraRotationSpeed * offset.y);
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		if (!m_Active)
			return;
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SW_BIND_EVENT_FN(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseMoved));
	}

	void CameraController::OnResize(float width, float height)
	{
		float aspect = width / height;
		switch (m_Camera->GetCameraType())
		{
		case CameraType::Orthographic:
			break;
		case CameraType::Perspective:
			CastRef<PerspCamera>(m_Camera)->SetAspect(aspect);
			break;
		default:
			SW_CORE_ASSERT(false, "Unknow CameraType");
		}
	}

	void CameraController::SetActive(bool active)
	{
		m_Active = active;
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset();
		switch (m_Camera->GetCameraType())
		{
		case CameraType::Orthographic:
			break;
		case CameraType::Perspective:
			CastRef<PerspCamera>(m_Camera)->SetFov(m_ZoomLevel);
			break;
		default:
			SW_CORE_ASSERT(false, "Unknow CameraType");
		}
		return true;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		auto aspect = (float)e.GetWidth() / (float)e.GetHeight();
		switch (m_Camera->GetCameraType())
		{
		case CameraType::Orthographic:
			break;
		case CameraType::Perspective:
			CastRef<PerspCamera>(m_Camera)->SetAspect(aspect);
			break;
		default:
			SW_CORE_ASSERT(false, "Unknow CameraType");
		}
		return true;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
			leftButtonDown = true;
		}
		return true;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			leftButtonDown = false;
		}
		return true;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (leftButtonDown)
		{
			auto [x, y] = e.GetPosition();
			float xOffset = x - lastX;
			float yOffset = lastY - y;

			m_Camera->SetYaw(m_Camera->GetYaw() + m_CameraRotationSpeed * xOffset);
			m_Camera->SetPitch(m_Camera->GetPitch() + m_CameraRotationSpeed * yOffset);

			lastX = x;
			lastY = y;
		}

		return true;
	}

}