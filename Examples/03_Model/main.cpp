#include <Schwi.h>
#include <Core/EntryPoint.h>

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
		m_Light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_Light.Bind(m_Shader, m_LightPos, 0);
		m_Shader->SetInt("PointLightNum", 1);

		m_LightShader = Shader::Create(SolutionDir + "assets/shaders/default.glsl");
		m_LightMesh = CreateRef<Mesh>(MeshType::Sphere, 2);
	}

	void OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		RenderCommand::SetLineMode(m_EnableLineMode);

		Renderer::BeginScene(m_CameraController.GetCamera());

		m_Light.Bind(m_Shader, m_LightPos, 0);
		m_Shader->SetInt("PointLightNum", 1);
		m_Shader->SetFloat3("u_ViewPos", m_CameraController.GetCamera()->GetPosition());

		m_Model->Draw(m_Shader);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_LightPos);
		transform = glm::scale(transform, glm::vec3(0.1f));
		m_LightMesh->Draw(m_LightShader, transform);

		Renderer::EndScene();
	}

	void OnEvent(Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		ImGui::SetCurrentContext(Application::Get().GetImGuiLayer()->GetCurrentContext().get());
		ImGui::Begin("Test");
		ImGui::Checkbox("Enable Line Mode", &m_EnableLineMode);
		ImGui::DragFloat3("Light Pos", (float*)&m_LightPos, 0.01f);
		ImGui::End();
	}
private:
	Ref<Shader> m_Shader, m_LightShader;
	Ref<VertexArray> m_LightVao;
	Ref<Model> m_Model;
	Ref<Mesh> m_LightMesh;
	CameraController m_CameraController;
	PointLight m_Light;

	bool m_EnableLineMode=false;
	glm::vec3 m_LightPos{ 1.0f,2.0f,1.0f };
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