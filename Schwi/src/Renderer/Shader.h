#pragma once

#include "Core/Core.h"

namespace schwi {
	class SCHWI_API Shader
	{
	public:
		virtual ~Shader() = default;

		static Ref<Shader> Create(const std::string& filepath);
		static Ref<Shader> Create(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual void Bind() const = 0;
		virtual void Unbind() const = 0;

		virtual uint32_t GetID() const = 0;

		virtual void SetInt(const std::string& name, int value) = 0;

		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetFloat4(const std::string& name, const glm::vec4& value) = 0;

		virtual void SetMat3(const std::string& name, const glm::mat3& matrix) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& matrix) = 0;
	};

}