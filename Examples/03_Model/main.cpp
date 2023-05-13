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
		m_Light.Bind(m_Shader, 0); 
		m_SpotLight.Bind(m_Shader, 0);
		m_Shader->SetInt("PointLightNum", 1);
		m_Shader->SetInt("SpotLightNum", 1);

		m_LightShader = Shader::Create(SolutionDir + "assets/shaders/default.glsl");
		m_LightMesh = CreateRef<Mesh>(MeshType::Sphere, 2);
	}

	void OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		RenderCommand::SetLineMode(m_EnableLineMode);

		Renderer::BeginScene(m_CameraController.GetCamera());

		m_Light.Bind(m_Shader,0);
		m_SpotLight.Bind(m_Shader,  0);
		m_Shader->SetInt("PointLightNum", 1);
		m_Shader->SetFloat3("u_ViewPos", m_CameraController.GetCamera()->GetPosition());

		m_Model->Draw(m_Shader);

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Light.position);
		transform = glm::scale(transform, glm::vec3(0.1f));
		m_LightMesh->Draw(m_LightShader, transform);

		transform = glm::translate(glm::mat4(1.0f), m_SpotLight.position);
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
		ImGui::DragFloat3("Point Light Pos", (float*)&m_Light.position, 0.01f);
		ImGui::DragFloat3("Spot Light Pos", (float*)&m_SpotLight.position, 0.01f);
		ImGui::SliderFloat3("Spot Light Dir", (float*)&m_SpotLight.Direction, -1.0f,1.0f);
		ImGui::End();
	}
private:
	Ref<Shader> m_Shader, m_LightShader;
	Ref<VertexArray> m_LightVao;
	Ref<Model> m_Model;
	Ref<Mesh> m_LightMesh;
	CameraController m_CameraController;
	PointLight m_Light;
	SpotLight m_SpotLight;

	bool m_EnableLineMode=false;
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