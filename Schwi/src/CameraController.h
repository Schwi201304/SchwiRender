#pragma once

#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"

namespace schwi {
	class CameraController
	{
	public:
		CameraController() = default;
		CameraController(Ref<Camera> camera) :m_Camera(camera) {}

		void OnUpdate(Timestep ts);
		void OnEvent(Event& e);

		Ref<Camera>& GetCamera() { return m_Camera; }
		const  Ref<Camera>& GetCamera() const { return m_Camera; }
	private:
		bool OnMouseScrolled(MouseScrolledEvent& e);
		bool OnWindowResized(WindowResizeEvent& e);
	private:
		Ref<Camera> m_Camera;

		float m_ZoomLevel = 45.0f;
		float m_CameraMoveSpeed = 5.0f;
		float m_CameraRotationSpeed = 90.0f;
	};

}