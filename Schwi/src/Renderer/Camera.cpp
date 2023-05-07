#include "swpch.h"
#include "Camera.h"

#include <glm/gtc/quaternion.hpp>

namespace schwi {
	void Camera::SetPosition(const glm::vec3& position)
	{
		if (m_Position == position)return;
		m_Position = position;
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetPitch(const float pitch)
	{
		if (m_Pitch == pitch)return;
		m_Pitch = std::min(std::max(pitch,-89.0f),89.0f);
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetYaw(const float yaw)
	{
		if (m_Yaw == yaw)return;
		m_Yaw = yaw;
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetNearPlane(const float& val)
	{
		if (m_NearPlane == val)return;
		m_NearPlane = val;
		UpdateProjectionMatrix();
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetFarPlane(const float& val)
	{
		if (m_FarPlane == val)return;
		m_FarPlane = val;
		UpdateProjectionMatrix();
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 front = glm::vec3(1.0f);

		front.x = cos(glm::radians(m_Pitch)) * cos(glm::radians(m_Yaw));
		front.y = sin(glm::radians(m_Pitch));
		front.z = cos(glm::radians(m_Pitch)) * sin(glm::radians(m_Yaw));
		m_Front = glm::normalize(front);

		// right vector
		m_Right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));

		// up vector
		m_Up = glm::normalize(glm::cross(m_Right, m_Front));

		m_ViewMatrix = glm::lookAt(m_Position, m_Front + m_Position, m_Up);
	}



	void PerspCamera::SetFov(float fov)
	{
		if (fov == m_Fov)return;
		m_Fov = fov;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}

	void PerspCamera::SetAspect(float aspect)
	{
		if (aspect = m_Aspect)return;
		m_Aspect = aspect;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}
}