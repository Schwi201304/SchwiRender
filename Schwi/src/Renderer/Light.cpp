#include "swpch.h"
#include "Light.h"

#include "Shader.h"

namespace schwi {
	void Light::Bind(const Ref<Shader>& shader, const uint32_t& slot)
	{
		shader->Bind();
		shader->SetFloat3("u_Light[" + std::to_string(slot) + "].color", Color);
		shader->SetFloat3("u_Light[" + std::to_string(slot) + "].position", Position);
		shader->SetFloat("u_Light[" + std::to_string(slot) + "].intensity", Intensity);
	}

	void DirLight::Bind(const Ref<Shader>& shader, const uint32_t& slot)
	{
		shader->Bind();
		shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].color", Color);
		shader->SetFloat3("u_DirLight[" + std::to_string(slot) + "].direction", Direction);
		shader->SetFloat("u_DirLight[" + std::to_string(slot) + "].intensity", Intensity);
	}

	void PointLight::Bind(const Ref<Shader>& shader, const uint32_t& slot)
	{
		shader->Bind();
		shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].color", Color);
		shader->SetFloat3("u_PointLight[" + std::to_string(slot) + "].position", Position);
		shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].constant", Constant);
		shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].linear", Linear);
		shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].quadratic", Quadratic);
		shader->SetFloat("u_PointLight[" + std::to_string(slot) + "].intensity", Intensity);
	}

	void SpotLight::Bind(const Ref<Shader>& shader, const uint32_t& slot)
	{
		shader->Bind();
		shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].color", Color);
		shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].position", Position);
		shader->SetFloat3("u_SpotLight[" + std::to_string(slot) + "].direction", Direction);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].cutoff", CutOff);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].outerCutOff", OuterCutOff);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].constant", Constant);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].linear", Linear);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].quadratic", Quadratic);
		shader->SetFloat("u_SpotLight[" + std::to_string(slot) + "].intensity", Intensity);
	}
}