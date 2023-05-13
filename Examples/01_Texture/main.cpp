#include <Schwi.h>
#include <Core/EntryPoint.h>

using namespace schwi;

class Example_Texture : public schwi::Layer
{
public:
	Example_Texture()
		: Layer("Example_Texture")
	{
		m_CameraController = schwi::CameraController(CreateRef<PerspCamera>(
			45.0f, Application::Get().GetWindow().GetAspect()));
		m_VertexArray = VertexArray::Create();

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

		m_SquareVA = VertexArray::Create();

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Ref<VertexBuffer> squareVB;
		squareVB = VertexBuffer::Create(squareVertices, sizeof(squareVertices));
		squareVB->SetLayout({
			{ schwi::ShaderDataType::Float3, "a_Position" },
			{ schwi::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Ref<schwi::IndexBuffer> squareIB;
		squareIB = schwi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t));
		m_SquareVA->SetIndexBuffer(squareIB);

		m_TextureShader = Shader::Create(SolutionDir +"assets/shaders/texture.glsl");

		m_Texture = Texture2D::Create(SolutionDir + "assets/textures/kq.png");

		m_TextureShader->Bind();
		m_TextureShader->SetInt("u_Texture", 0);
	}

	void OnUpdate(Timestep ts) override
	{
		m_CameraController.OnUpdate(ts);

		Renderer::BeginScene(m_CameraController.GetCamera());

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_Texture->Bind();
		Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		Renderer::EndScene();
	}

	void OnEvent(Event& e) override
	{
		m_CameraController.OnEvent(e);
	}

	virtual void OnImGuiRender() override
	{
	}

private:
	Ref<Shader> m_Shader;
	Ref<VertexArray> m_VertexArray;

	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	Ref<Shader> m_FlatColorShader, m_TextureShader;
	Ref<VertexArray> m_SquareVA;
	Ref<Texture2D> m_Texture;

	schwi::CameraController m_CameraController;
};

class APP : public schwi::Application
{
public:
	APP()
	{
		PushLayer(CreateRef<Example_Texture>());
	}

	~APP()
	{

	}

};

schwi::Application* schwi::CreateApplication()
{
	return new APP();
}