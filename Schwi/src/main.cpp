#include "swpch.h"
#include "Schwi.h"

using namespace schwi;
class Example_Texture : public schwi::Layer
{
public:
	Example_Texture()
		: Layer("Example_Texture")
	{
		m_CameraController = schwi::CameraController(CreateRef<schwi::PerspCamera>(
			45.0f, schwi::Application::Get().GetWindow().GetAspect()));
		m_VertexArray = schwi::VertexArray::Create();

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Ref<schwi::VertexBuffer> vertexBuffer;
		vertexBuffer = VertexBuffer::Create(vertices, sizeof(vertices));
		schwi::BufferLayout layout = {
			{ schwi::ShaderDataType::Float3, "a_Position" },
			{ schwi::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Ref<schwi::IndexBuffer> indexBuffer;
		indexBuffer = schwi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA=schwi::VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<schwi::VertexBuffer> squareVB;
		squareVB = schwi::VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ schwi::ShaderDataType::Float3, "a_Position" },
			{ schwi::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<schwi::IndexBuffer> squareIB;
		squareIB = schwi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_TextureShader = schwi::Shader::Create("assets/shaders/texture.glsl");

		m_Texture = schwi::Texture2D::Create("assets/textures/kq.png");

		m_TextureShader->Bind();
		m_TextureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(schwi::Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		schwi::RenderCommand::SetClearColor(m_ClearColor);
		schwi::RenderCommand::Clear();

		schwi::Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_Texture->Bind();
		schwi::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		schwi::Renderer::EndScene();
	}

	void OnEvent(schwi::Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
		static float f = 0.0f;
		static int counter = 0;
		ImGui::Begin("Test");
		ImGui::Text("Hello World");
		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
		if (ImGui::Button("Button"))
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);
		ImGui::ColorEdit4("clear color", (float*)&m_ClearColor);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		ImGui::End();
	}

private:
	Ref<schwi::Shader> m_Shader;
	Ref<schwi::VertexArray> m_VertexArray;

	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	Ref<schwi::Shader> m_FlatColorShader, m_TextureShader;
	Ref<schwi::VertexArray> m_SquareVA;
	Ref<schwi::Texture2D> m_Texture;

	schwi::CameraController m_CameraController;

	glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 1.f };
};

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
		m_LightShader = schwi::Shader::Create("assets/shaders/default.glsl");

		m_Shader = Shader::Create("assets/shaders/light.glsl");
		m_Texture = Texture2D::Create("assets/textures/kq.png");
		m_Material.DiffuseTexture = m_Texture;
		m_Material.SpecularTexture = m_Texture;
		m_Material.m_Shader = m_Shader;
		m_Material.Bind();
		m_Light.Bind(m_Shader, m_LightPos, 0);
		m_Shader->SetInt("PointLightNum", 1);
	}

	void OnUpdate(Timestep ts)
	{
		m_CameraController.OnUpdate(ts);

		RenderCommand::SetClearColor(m_ClearColor);
		RenderCommand::Clear();

		Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_LightPos);
		transform = glm::scale(transform,glm::vec3(0.1f));

		m_Material.DiffuseTexture = m_Texture;
		m_Material.Bind();
		m_Light.Bind(m_Shader, m_LightPos, 0);
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
	{}


private:
	Ref<Shader> m_Shader, m_LightShader;
	Ref<Texture2D> m_Texture;
	Ref<VertexArray> m_VertexArray, m_LightVao;
	CameraController m_CameraController;
	PointLight m_Light;
	PhongMaterial m_Material;

	glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 1.f };
	//glm::vec4 m_ClearColor{ 0, 0, 0, 1.f };
	glm::vec3 m_LightPos{ 1.0f,1.0f,2.0f };
};

class Sandbox : public schwi::Application
{
public:
	Sandbox()
	{
		//PushLayer(new Example_Texture());
		PushLayer(new Example_Light());
	}

	~Sandbox()
	{
	}
};

Application* schwi::CreateApplication()
{
	return new Sandbox();
}

int main()
{
	schwi::Log::Init();
	std::cout << "Hello!" << std::endl;
	auto app = schwi::CreateApplication();
	app->Run();

	app = nullptr;
	return 0;
}