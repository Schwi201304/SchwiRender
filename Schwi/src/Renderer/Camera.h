#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace schwi {

	enum CameraType
	{
		CameraTypeNone = 0,
		Orthographic = 1,
		Perspective = 2
	};

	class Camera
	{
	public:
		Camera() = default;
		Camera(float nearPlane, float farPlane, CameraType cameraType = CameraType::Perspective)
			:m_NearPlane(nearPlane), m_FarPlane(farPlane), m_CameraType(cameraType) {}
		virtual ~Camera() = default;

		const CameraType& GetCameraType() { return m_CameraType; };

		const float& GetPitch() const { return m_Pitch; }
		const float& GetYaw() const { return m_Yaw; }

		const glm::vec3& GetPosition() const { return m_Position; }
		const glm::vec3& GetUpVector() const { return m_Up; }
		const glm::vec3& GetFrontVector() const { return m_Front; }
		const glm::vec3& GetRightVector() const { return m_Right; }

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

		void SetPosition(const glm::vec3& position);
		void SetPitch(const float pitch);
		void SetYaw(const float yaw);
		void SetNearPlane(const float& val);
		void SetFarPlane(const float& val);

	protected:
		void UpdateViewMatrix();
		virtual void UpdateProjectionMatrix() = 0;
		void UpdateViewProjectionMatrix() { m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix; }

	protected:
		CameraType m_CameraType;
		float m_FarPlane, m_NearPlane;
		float m_Pitch = 0.0f, m_Yaw = -90.0f;
		glm::vec3 m_Position = { 0.0f, 0.0f, 5.0f };
		glm::mat4 m_ViewMatrix;
		glm::mat4 m_ProjectionMatrix;
		glm::mat4 m_ViewProjectionMatrix;
		glm::vec3 m_Front = { 0.0f, 0.0f, -1.0f }, m_Up = { 0.0f, 1.0f, 0.0f }, m_Right = { 1.0f, 0.0f, 0.0f };
	};

	class OrthoCamera :public Camera
	{
	public:
		OrthoCamera(float left, float right, float bottom, float top, float nearPlane = 0.1f, float farPlane = 100.0f)
			:m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top),
			Camera(nearPlane, farPlane, CameraType::Orthographic)
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateViewProjectionMatrix();
		}
		~OrthoCamera() = default;

		const float& GetLeft() const { return m_Left; }
		const float& GetRight() const { return m_Right; }
		const float& GetBottom() const { return m_Bottom; }
		const float& GetTop() const { return m_Top; }


	private:
		virtual void UpdateProjectionMatrix() override
		{
			m_ProjectionMatrix = glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_NearPlane, m_FarPlane);
		}

	private:
		float m_Left, m_Right, m_Bottom, m_Top;
	};

	class PerspCamera :public Camera
	{
	public:
		PerspCamera(float fov, float aspect, float nearPlane = 0.1f, float farPlane = 100.0f)
			:m_Fov(fov), m_Aspect(aspect),
			Camera(nearPlane, farPlane, CameraType::Perspective)
		{
			UpdateViewMatrix();
			UpdateProjectionMatrix();
			UpdateViewProjectionMatrix();
		}
		~PerspCamera() = default;

		const float& GetFov() const { return m_Fov; }
		const float& GetAspect() const { return m_Aspect; }

		void SetFov(float fov);
		void SetAspect(float aspect);

	private:
		virtual void UpdateProjectionMatrix() override
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_Aspect, m_NearPlane, m_FarPlane);
		}

	private:
		float m_Fov, m_Aspect;
	};
}