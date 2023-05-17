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
		m_Pitch = std::min(std::max(pitch, -89.0f), 89.0f);
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetYaw(const float yaw)
	{
		if (m_Yaw == yaw)return;
		m_Yaw = std::min(std::max(yaw, -180.0f), 180.0f);
		UpdateViewMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetNearPlane(const float val)
	{
		if (m_NearPlane == val || val <= 0)return;
		m_NearPlane = val;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetFarPlane(const float val)
	{
		if (m_FarPlane == val || val <= 0)return;
		m_FarPlane = val;
		UpdateProjectionMatrix();
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

	void Camera::UpdateProjectionMatrix()
	{
		switch (m_CameraType)
		{
		case CameraType::Orthographic:
			m_ProjectionMatrix = glm::ortho(
				-m_Height * m_Aspect / 2.0f, m_Height * m_Aspect / 2.0f,
				-m_Height / 2.0f, m_Height / 2.0f, m_NearPlane, m_FarPlane);
			break;
		case CameraType::Perspective:
			m_ProjectionMatrix = glm::perspective(
				glm::radians(m_Fov), m_Aspect, m_NearPlane, m_FarPlane);
			break;
		}
	}

	void Camera::SetAspect(const float aspect)
	{
		if (aspect == m_Aspect)return;
		m_Aspect = aspect;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetHeight(float val)
	{
		if (val == m_Height)return;
		m_Height = val;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}

	void Camera::SetFov(float fov)
	{
		if (fov == m_Fov || fov <= 0 || fov >= 120)return;
		m_Fov = fov;
		UpdateProjectionMatrix();
		UpdateViewProjectionMatrix();
	}
}