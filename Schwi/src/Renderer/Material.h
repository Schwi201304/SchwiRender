#pragma once

#include "Core/Core.h"

namespace schwi {
	class Shader;
	class Texture2D;

	enum class MaterialType
	{
		Material_None = 0,
		Material_Emission = 1,
		Material_Phong = 2,

	};

	struct Material
	{
		Ref<Shader> m_Shader = nullptr;
		Material() = default;
		virtual ~Material() {}
		virtual void ResetTexture(Ref<Texture2D>& texture, const std::string path = std::string());
		virtual void Bind();
	};


	struct PhongMaterial :public Material
	{
		Ref<Shader> m_Shader = nullptr;
		Ref<Texture2D> DiffuseTexture = nullptr;
		Ref<Texture2D> SpecularTexture = nullptr;
		Ref<Texture2D> NormalTexture = nullptr;
		Ref<Texture2D> DisplacementTexture = nullptr;
		float HeightScale = 0;
		glm::vec3 Color{ 1.0f };
		float Shininess = 32;
		//PhongMaterial()
		//{
		//	ResetTexture(DiffuseTexture);
		//	ResetTexture(SpecularTexture);
		//	ResetTexture(NormalTexture);
		//	ResetTexture(DisplacementTexture);
		//}
		virtual ~PhongMaterial() {}
		virtual void Bind() override;
	};
}