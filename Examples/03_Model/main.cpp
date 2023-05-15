#include <Schwi.h>
#include <Core/EntryPoint.h>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

using namespace schwi;

class Example_Model :public Layer
{
public:
	Example_Model() :Layer("Example_Model")
	{
		m_CameraController = CameraController(CreateRef<schwi::PerspCamera>(
			45.0f, Application::Get().GetWindow().GetAspect()));

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

	void OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		RenderCommand::SetLineMode(m_EnableLineMode);

		Renderer::BeginScene(m_CameraController.GetCamera());

		m_Light.Bind(m_Shader, 0);
		m_SpotLight.Bind(m_Shader, 0);
		m_Shader->SetFloat3("u_ViewPos", m_CameraController.GetCamera()->GetPosition());

		//Point Light
		m_DefaultShader->Bind();
		m_DefaultShader->SetFloat4("u_FragColor", glm::vec4(1.0f));
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

	void OnEvent(Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::SetCurrentContext(Application::Get().GetImGuiLayer()->GetCurrentContext().get());
		ImGui::Begin("Test");
		ImGui::Checkbox("Draw outline", &m_Model->m_DrawOutline);
		ImGui::Checkbox("Enable Line Mode", &m_EnableLineMode);
		ImGui::DragFloat3("Point Light Pos", (float*)&m_Light.Position, 0.01f);
		ImGui::DragFloat3("Spot Light Pos", (float*)&m_SpotLight.Position, 0.01f);
		ImGui::SliderFloat3("Spot Light Dir", (float*)&m_SpotLight.Direction, -1.0f, 1.0f);
		ImGui::End();
	}
private:
	Ref<Shader> m_Shader, m_DefaultShader;
	Ref<VertexArray> m_LightVao;
	Ref<Model> m_Model;
	Ref<Mesh> m_LightMesh, m_PlaneMesh;
	CameraController m_CameraController;
	PointLight m_Light;
	SpotLight m_SpotLight;

	bool m_EnableLineMode = false;
};

class APP : public schwi::Application
{
public:
	APP()
	{
		PushLayer(CreateRef<Example_Model>());
	}

	~APP()
	{

	}

};

schwi::Application* schwi::CreateApplication()
{
	return new APP();
}