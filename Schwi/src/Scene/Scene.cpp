#include "swpch.h"
#include "Scene.h"
#include "SceneLayer.h"

namespace schwi {
	Ref<Scene::SceneData> Scene::s_SceneData = CreateRef<SceneData>();



	Scene::Scene()
	{
		auto camera = CreateRef<schwi::Camera>(0.1f, 100.0f, SceneLayer::GetInstance()->GetAspect());
		camera->SetPersp(45.0f);
		m_CameraController = CreateRef<CameraController>(camera);		

		m_ModelList.push_back(CreateRef<Model>(SolutionDir + "assets/models/nanosuit/nanosuit.obj"));

		m_Shader = Shader::Create(SolutionDir + "assets/shaders/phong.glsl");
		
		m_PointLightList.push_back(CreateRef<PointLight>());
		for (auto& light : m_PointLightList)
		{
			light->Bind(m_Shader, 0);
		}
		m_Shader->SetInt("PointLightNum", m_PointLightList.size());
		sphere = CreateRef<Mesh>(MeshType::Sphere, 1);
		plane = CreateRef<Mesh>(MeshType::Plane, 1);
		m_DefaultShader = Shader::Create(SolutionDir + "assets/shaders/default.glsl");
	}

	void Scene::Draw()
	{
		RenderCommand::SetLineMode(m_EnableLineMode);

		s_SceneData->ViewProjectionMatrix = m_CameraController->GetCamera()->GetViewProjectionMatrix();

		for (auto& light : m_PointLightList)
		{
			light->Bind(m_Shader, 0);
		}
		m_Shader->SetInt("PointLightNum", m_PointLightList.size());

		m_Shader->SetFloat3("u_ViewPos", m_CameraController->GetCamera()->GetPosition());

		//Point Light
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f));
		m_DefaultShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		int numLight=0;
		for (const auto& light : m_PointLightList)
		{
			light->Bind(m_Shader, numLight++);
			glm::mat4 transform = glm::translate(glm::mat4(1.0f), light->Position);
			transform = glm::scale(transform, glm::vec3(0.05f));
			sphere->Draw(m_DefaultShader, transform);
		}
		//Floor
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		plane->Draw(m_DefaultShader, glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
		//Model
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		for (const auto& model : m_ModelList)
		{
			model->Draw(m_Shader);
		}
		//Glass
		//m_DefaultShader->Bind();
		//m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
		//transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
		//transform = glm::scale(transform, glm::vec3(5.0f));
		//transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		//m_PlaneMesh->Draw(m_DefaultShader, transform);

		for (const auto& model : m_ModelList)
		{
			model->DrawOutline();
		}
	}
}