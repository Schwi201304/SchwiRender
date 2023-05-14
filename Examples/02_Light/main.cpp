#include <Schwi.h>
#include <Core/EntryPoint.h>

using namespace schwi;

class Example_Light : public Layer
{
public:
	Example_Light() :Layer("Example_Light")
	{
		m_CameraController = CameraController(CreateRef<schwi::PerspCamera>(
			45.0f, Application::Get().GetWindow().GetAspect()));
		m_VertexArray = VertexArray::Create();

		float vertices[] = {
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f, 0.0f, 1.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f, 0.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 0.0f,  0.0f,  0.0f,  1.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, 0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f, 0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f, 1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, 0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f, 0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f, 1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, 0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		};

		Ref<VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		BufferLayout layout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float2, "a_TexCoord" },
			{ ShaderDataType::Float3, "a_Normal"}
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[] = {
			0,1,2,2,3,0,
			4,5,6,6,7,4,
			8,9,10,10,11,8,
			12,13,14,14,15,12,
			16,17,18,18,19,16,
			20,21,22,22,23,20
		};

		Ref<IndexBuffer> indexBuffer;
		indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_LightVao = VertexArray::Create();
		m_LightVao->AddVertexBuffer(vertexBuffer);
		m_LightVao->SetIndexBuffer(indexBuffer);
		m_LightShader = schwi::Shader::Create(SolutionDir + "assets/shaders/default.glsl");

		m_Shader = Shader::Create(SolutionDir + "assets/shaders/light.glsl");
		m_Texture = Texture2D::Create(SolutionDir + "assets/textures/kq.png");
		m_Material.DiffuseTexture = m_Texture;
		m_Material.SpecularTexture = m_Texture;
		m_Material.m_Shader = m_Shader;
		m_Material.Bind();
		m_Light.Color = glm::vec3(1.0f, 1.0f, 1.0f);
		m_Light.Bind(m_Shader, 0);
		m_Shader->SetInt("PointLightNum", 1);
	}

	void OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Light.Position);
		transform = glm::scale(transform, glm::vec3(0.1f));

		m_Material.DiffuseTexture = m_Texture;
		m_Material.Bind();
		m_Light.Bind(m_Shader, 0);
		m_Shader->SetInt("PointLightNum", 1);
		m_Shader->SetFloat3("u_ViewPos", m_CameraController.GetCamera()->GetPosition());
		Renderer::Submit(m_Shader, m_VertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Renderer::Submit(m_LightShader, m_LightVao, transform);

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
		ImGui::DragFloat3("Light Pos", (float*)&m_Light.Position, 0.01f);
		ImGui::End();
	}


private:
	Ref<Shader> m_Shader, m_LightShader;
	Ref<Texture2D> m_Texture;
	Ref<VertexArray> m_VertexArray, m_LightVao;
	CameraController m_CameraController;
	PointLight m_Light;
	PhongMaterial m_Material;
};

class APP : public schwi::Application
{
public:
	APP()
	{
		PushLayer(CreateRef<Example_Light>());
	}

	~APP()
	{

	}

};

schwi::Application* schwi::CreateApplication()
{
	return new APP();
}