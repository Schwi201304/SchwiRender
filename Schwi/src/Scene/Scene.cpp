#include "swpch.h"
#include "Scene.h" 

#include "Renderer/Shader.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Model.h"
#include "Renderer/Light.h"
#include "CameraController.h"
#include "SceneLayer.h"
#include "Components.h"
#include "Renderer/RenderCommand.h"

namespace schwi {
	Ref<Scene::SceneData> Scene::s_SceneData = CreateRef<SceneData>();

	Scene::Scene()
	{
		m_Shader = Shader::Create(SolutionDir + "assets/shaders/phong.glsl");
		m_DefaultShader = Shader::Create(SolutionDir + "assets/shaders/default.glsl");
	}

	void Scene::DrawLight()
	{
		auto group = m_Registry.view<TransformComponent, LightComponent>();
		size_t num = 0;
		for (auto entity : group)
		{
			auto [transform, light] = group.get<TransformComponent, LightComponent>(entity);
			light.light->Position = transform.Translation;
			light.light->Bind(m_Shader, num++);
		}
		m_Shader->SetInt("PointLightNum", num);

		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f));
		m_DefaultShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
	}

	void Scene::Draw(const Ref<Camera>& camera)
	{
		RenderCommand::SetLineMode(m_EnableLineMode);

		s_SceneData->ViewProjectionMatrix = camera->GetViewProjectionMatrix();

		DrawLight();
		m_Shader->Bind();
		m_Shader->SetFloat3("u_ViewPos", camera->GetPosition());
		m_Shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		auto group = m_Registry.view< TransformComponent, ModelComponent>();
		for (auto entity : group)
		{
			auto [transform, model] = group.get<TransformComponent, ModelComponent>(entity);
			model.model->SetTransform(transform.GetTransform());
			model.model->Draw(m_Shader);
		}
	}
}