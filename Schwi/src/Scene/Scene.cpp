#include "swpch.h"
#include "Scene.h"

#include "SceneLayer.h"

namespace schwi {
	Ref<Scene::SceneData> Scene::s_SceneData = CreateRef<SceneData>();

	Scene::Scene()
	{
		m_CameraController = CreateRef<CameraController>(CreateRef<schwi::PerspCamera>(
			45.0f, SceneLayer::GetInstance()->GetAspect()));

		m_Model = CreateRef<Model>(SolutionDir + "assets/models/nanosuit/nanosuit.obj", glm::scale(glm::mat4(1.0f), glm::vec3(0.5f)));

		m_Shader = Shader::Create(SolutionDir + "assets/shaders/phong.glsl");
		m_Light.Bind(m_Shader, 0);
		m_Light.Position = glm::vec3(2.0f, 5.0f, 2.0f);
		m_SpotLight.Bind(m_Shader, 0);
		m_SpotLight.Position = glm::vec3(0.0f, 9.0f, 0.0f);
		m_Shader->SetInt("PointLightNum", 1);
		m_Shader->SetInt("SpotLightNum", 1);

		m_DefaultShader = Shader::Create(SolutionDir + "assets/shaders/default.glsl");

		m_LightMesh = CreateRef<Mesh>(MeshType::Sphere, 2);
		m_PlaneMesh = CreateRef<Mesh>(MeshType::Plane, 5);
	}

	void Scene::Draw()
	{
		RenderCommand::SetLineMode(m_EnableLineMode);

		s_SceneData->ViewProjectionMatrix = m_CameraController->GetCamera()->GetViewProjectionMatrix();

		m_Light.Bind(m_Shader, 0);
		m_SpotLight.Bind(m_Shader, 0);
		m_Shader->SetFloat3("u_ViewPos", m_CameraController->GetCamera()->GetPosition());

		//Point Light
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f));
		m_DefaultShader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Light.Position);
		transform = glm::scale(transform, glm::vec3(0.05f));
		m_LightMesh->Draw(m_DefaultShader, transform);
		//Spot Light
		transform = glm::translate(glm::mat4(1.0f), m_SpotLight.Position);
		transform = glm::scale(transform, glm::vec3(0.05f));
		m_LightMesh->Draw(m_DefaultShader, transform);
		//Floor
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
		m_PlaneMesh->Draw(m_DefaultShader, glm::scale(glm::mat4(1.0f), glm::vec3(10.0f)));
		//Model
		m_Shader->Bind();
		m_Shader->SetMat4("u_ViewProjection", s_SceneData->ViewProjectionMatrix);
		m_Model->Draw(m_Shader);
		//Glass
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f, 0.0f, 0.0f, 0.2f));
		transform = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 3.0f));
		transform = glm::scale(transform, glm::vec3(5.0f));
		transform = glm::rotate(transform, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		m_PlaneMesh->Draw(m_DefaultShader, transform);

		m_Model->DrawOutline();
	}
}