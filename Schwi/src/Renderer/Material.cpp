#include "swpch.h"
#include "Material.h"

#include "Shader.h"
#include "Texture.h"

namespace schwi {
	void Material::ResetTexture(Ref<Texture2D>& texture, const std::string path)
	{
		texture.reset();
		if (!path.empty())
			texture = Texture2D::Create(path);
	}

	void Material::Bind()
	{
		m_Shader->Bind();
	}

	void PhongMaterial::Bind()
	{
		m_Shader->Bind();
		if (DiffuseTexture)
		{
			DiffuseTexture->Bind(0);
			m_Shader->SetInt("u_Material.diffuse", 0);
		}
		if (SpecularTexture)
		{
			SpecularTexture->Bind(1);
			m_Shader->SetInt("u_Material.specular", 1);
		}
		if (NormalTexture)
		{
			NormalTexture->Bind(2);
			m_Shader->SetInt("u_Material.normalMap", 2);
		}
		if (DisplacementTexture)
		{
			DisplacementTexture->Bind(3);
			m_Shader->SetInt("u_Material.displacementMap", 3);
		}
		m_Shader->SetFloat3("u_Material.color", Color);
		m_Shader->SetFloat("u_HeightScale", HeightScale);
		m_Shader->SetFloat("u_Material.shininess", Shininess);
	}
}