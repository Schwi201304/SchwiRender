#pragma once

#include "Core/Core.h"

namespace schwi {

	enum class SCHWI_API CameraType
	{
		Orthographic = 0,
		Perspective = 1
	};

	class SCHWI_API Camera
	{
	public:
		Camera()
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateViewProjectionMatrix();
		}
		Camera(float nearPlane, float farPlane, float aspect, CameraType cameraType = CameraType::Perspective)
			:m_NearPlane(nearPlane), m_FarPlane(farPlane), m_Aspect(aspect), m_CameraType(cameraType)
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateViewProjectionMatrix();
		}
		~Camera() = default;

		const CameraType& GetCameraType() { return m_CameraType; };
		void SetCameraType(CameraType cameraType) 
		{ 
			m_CameraType = cameraType;
			UpdateProjectionMatrix();
			UpdateViewProjectionMatrix();
		}
		void SetPersp(float fov)
		{
			m_CameraType = CameraType::Perspective;
			SetFov(fov);
		}
		void SetOrtho(float height)
		{
			m_CameraType = CameraType::Orthographic;
			SetHeight(height);
		}

		const float GetPitch() const { return m_Pitch; }
		const float GetYaw() const { return m_Yaw; }
		const float GetAspect()const { return m_Aspect; }
		const float GetHeight() const { return m_Height; }
		const float GetFov() const { return m_Fov; }
		const float GetNearPlane()const { return m_NearPlane; }
		const float GetFarPlane()const { return m_FarPlane; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3 GetRotate()const { return { m_Pitch,m_Yaw,0.0f }; }
		const glm::vec3& GetUpVector() const { return m_Up; }
		const glm::vec3& GetFrontVector() const { return m_Front; }
		const glm::vec3& GetRightVector() const { return m_Right; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position);
		void SetPitch(const float pitch);
		void SetYaw(const float yaw);
		void SetNearPlane(const float val);
		void SetFarPlane(const float val);
		void SetAspect(const float val);
		void SetHeight(float val);
		void SetFov(float fov);

	private:
		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix();
		void UpdateViewProjectionMatrix() { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }

	private:
		CameraType m_CameraType = CameraType::Perspective;
		float m_FarPlane=100.0f, m_NearPlane=0.1f, m_Aspect=1.0f;
		float m_Pitch = 0.0f, m_Yaw = -90.0f;
		glm::vec3 m_Position = { 0.0f, 3.0f, 5.0f };
		glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
		glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f }, m_Up = { 0.0f, 1.0f, 0.0f }, m_Right = { 1.0f, 0.0f, 0.0f };

		float m_Height = 10.0f;
		float m_Fov = 45.0f;

	};
}