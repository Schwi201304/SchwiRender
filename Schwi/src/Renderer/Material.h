#pragma once
#include "swpch.h"
#include "Shader.h"
#include "Texture.h"

namespace schwi {
	enum class MaterialType
	{
		Material_None = 0,
		Material_Emission = 1,
		Material_Phong = 2,

	};

	struct Material
	{
		Ref<Shader> m_Shader = Shader::Create("assets/shaders/default.glsl");
		Material() = default;
		virtual ~Material() {}
		virtual void ResetTexture(Ref<Texture2D>& texture, const std::string path = std::string())
		{
			texture.reset();
			texture = Texture2D::Create(path);
		}
		virtual void Bind() { m_Shader->Bind(); };
	};


	struct PhongMaterial :public Material
	{
		Ref<Shader> m_Shader = Shader::Create("assets/shaders/phong.glsl");
		Ref<Texture2D> DiffuseTexture = nullptr;
		Ref<Texture2D> SpecularTexture = nullptr;
		Ref<Texture2D> NormalTexture = nullptr;
		Ref<Texture2D> DisplacementTexture = nullptr;
		float HeightScale = 0;
		glm::vec3 Color{ 1.0f };
		float Shininess = 32;
		PhongMaterial()
		{
			ResetTexture(DiffuseTexture);
			ResetTexture(SpecularTexture);
			ResetTexture(NormalTexture);
			ResetTexture(DisplacementTexture);
		}
		virtual ~PhongMaterial() {}
		virtual void Bind() override
		{
			m_Shader->Bind();
			DiffuseTexture->Bind(0);
			m_Shader->SetInt("u_Material.diffuse", 0);
			SpecularTexture->Bind(1);
			m_Shader->SetInt("u_Material.specular", 1);
			NormalTexture->Bind(2);
			m_Shader->SetInt("u_Material.normalMap", 2);
			DisplacementTexture->Bind(3);
			m_Shader->SetInt("u_Material.displacementMap", 3);
			m_Shader->SetFloat3("u_Material.color", Color);
			m_Shader->SetFloat("u_HeightScale", HeightScale);
			m_Shader->SetFloat("u_Material.shininess", Shininess);
		}
	};
}