#pragma once
#include "Renderer/Shader.h"
#include "Core/Core.h"
#include <string>
#include <unordered_map>

namespace schwi {
	class SCHWI_API OpenGLShader : public Shader
	{
	public:
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexSrc, const std::string& fragmentSrc);
		virtual ~OpenGLShader();

		virtual void Bind() const override;
		virtual void Unbind() const override;

		void SetInt(const std::string& name, int value);

		void SetFloat(const std::string& name, float value);
		void SetFloat2(const std::string& name, const glm::vec2& value);
		void SetFloat3(const std::string& name, const glm::vec3& value);
		void SetFloat4(const std::string& name, const glm::vec4& value);

		void SetMat3(const std::string& name, const glm::mat3& matrix);
		void SetMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;

	private:
		std::string ReadFile(const std::string& filepath);
		std::unordered_map<uint32_t, std::string> PreProcess(const std::string& source);
		void Compile(const std::unordered_map<uint32_t, std::string>& shaderSources);
	};
}