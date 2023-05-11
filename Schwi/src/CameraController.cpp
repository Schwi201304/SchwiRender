#include "swpch.h"
#include "CameraController.h"

#include "Core/Input.h"
#include "Core/Code.h"
#include "Core/Core.h"

namespace schwi {

	void CameraController::OnUpdate(Timestep ts)
	{
		if (schwi::Input::IsKeyPressed(schwi::Key::W))
			m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (schwi::Input::IsKeyPressed(schwi::Key::S))
			m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (schwi::Input::IsKeyPressed(schwi::Key::A))
			m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraMoveSpeed * ts * m_Camera->GetRightVector());
		if (schwi::Input::IsKeyPressed(schwi::Key::D))
			m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraMoveSpeed * ts * m_Camera->GetRightVector());
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(SW_BIND_EVENT_FN(CameraController::OnWindowResized));
		dispatcher.Dispatch<MouseButtonPressedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseButtonPressed));
		dispatcher.Dispatch<MouseButtonReleasedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseButtonReleased));
		dispatcher.Dispatch<MouseMovedEvent>(SW_BIND_EVENT_FN(CameraController::OnMouseMoved));
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
			SW_ASSERT(false, "Unknow CameraType");
		}
		return false;
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
			SW_ASSERT(false, "Unknow CameraType");
		}
		return false;
	}

	bool CameraController::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			lastX = Input::GetMouseX();
			lastY = Input::GetMouseY();
			leftButtonDown = true;
		}
		return false;
	}

	bool CameraController::OnMouseButtonReleased(MouseButtonReleasedEvent& e)
	{
		if (e.GetMouseButton() == Mouse::ButtonLeft)
		{
			leftButtonDown = false;
		}
		return false;
	}

	bool CameraController::OnMouseMoved(MouseMovedEvent& e)
	{
		if (leftButtonDown)
		{
			auto [x, y] = e.GetPosition();
			float xOffset = lastX - x;
			float yOffset = y - lastY;

			m_Camera->SetYaw(m_Camera->GetYaw() + m_CameraRotationSpeed * xOffset);
			m_Camera->SetPitch(m_Camera->GetPitch() + m_CameraRotationSpeed * yOffset);

			lastX = x;
			lastY = y;
		}

		return false;
	}

}