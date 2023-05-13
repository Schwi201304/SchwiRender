#pragma once

#include <string>
#include "Core/Core.h"
#include "Camera.h"
#include "Shader.h"

namespace schwi {
	enum class LightType
	{
		LightType_None = 0,
		LightType_Basic = 1,
		LightType_DirLight = 2,
		LightType_PointLight = 3,
		LightType_SpotLight = 4
	};

	struct Light
	{
		glm::vec3 Color{ 1.0f };
		glm::vec3 position{ 2.0f };
		float Intensity = 1.0f;

		Light() = default;

		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot)
		{
			shader->Bind();
			shader->SetFloat3("u_Light[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_Light[" + std::to_string(slot) + "].position", position);
			shader->SetFloat("u_Light[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

	struct DirLight : public Light
	{
		glm::vec3 Direction{ 1.0f };

		DirLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].direction", Direction);
			shader->SetFloat("u_DirLight[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

	struct PointLight : public Light
	{
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		PointLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", position);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
			shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

	struct SpotLight : public Light
	{
		glm::vec3  Direction{ 0.0f,-1.0f,0.0f };
		float CutOff = glm::cos(glm::radians(12.5f));
		float OuterCutOff = glm::cos(glm::radians(17.5f));
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		SpotLight() = default;

		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override
		{
			shader->Bind();
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].color", Color);
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].position", position);
			shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].direction", Direction);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].cutoff", CutOff);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].outerCutOff", OuterCutOff);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].constant", Constant);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].linear", Linear);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].quadratic", Quadratic);
			shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].intensity", Intensity);
		}
	};

}