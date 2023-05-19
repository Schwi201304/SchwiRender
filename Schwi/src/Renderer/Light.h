#pragma once

#include "Core/Core.h"

namespace schwi {
	class Shader;

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
		glm::vec3 Position{ 2.0f };
		float Intensity = 1.0f;

		Light() = default;

		virtual LightType GetLightType() {return LightType::LightType_Basic;}
		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot);
	};

	struct DirLight : public Light
	{
		glm::vec3 Direction{ 1.0f };

		DirLight() = default;

		virtual LightType GetLightType() { return LightType::LightType_DirLight; }
		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override;
	};

	struct PointLight : public Light
	{
		float Constant = 1.0f;
		float Linear = 0.09f;
		float Quadratic = 0.032f;

		PointLight() = default;

		virtual LightType GetLightType() { return LightType::LightType_PointLight; }
		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override;
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

		virtual LightType GetLightType() { return LightType::LightType_SpotLight; }
		virtual void Bind(const Ref<Shader>& shader, const uint32_t& slot) override;
	};

}