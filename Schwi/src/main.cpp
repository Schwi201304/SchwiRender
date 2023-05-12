#include "swpch.h"
#include "Schwi.h"

class ExampleLayer : public schwi::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
		m_Camera = std::make_shared<schwi::PerspCamera>(45.0f, schwi::Application::Get().GetWindow().GetAspect());
		m_VertexArray.reset(schwi::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<schwi::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(schwi::VertexBuffer::Create(vertices, sizeof(vertices)));
		schwi::BufferLayout layout = {
			{ schwi::ShaderDataType::Float3, "a_Position" },
			{ schwi::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<schwi::IndexBuffer> indexBuffer;
		indexBuffer.reset(schwi::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(schwi::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		std::shared_ptr<schwi::VertexBuffer> squareVB;
		squareVB.reset(schwi::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ schwi::ShaderDataType::Float3, "a_Position" },
			{ schwi::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<schwi::IndexBuffer> squareIB;
		squareIB.reset(schwi::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(schwi::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec3 v_Position;

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			uniform vec3 u_color;
			in vec3 v_Position;
			void main()
			{
				color = vec4(u_color, 1.0);
			}
		)";

		m_FlatColorShader.reset(schwi::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec2 v_TexCoord;
			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;
			
			uniform sampler2D u_Texture;
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(schwi::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = schwi::Texture2D::Create("assets/textures/kq.png");

		std::dynamic_pointer_cast<schwi::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<schwi::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(schwi::Timestep ts) override
	{
		if (schwi::Input::IsKeyPressed(schwi::Key::W))
			m_Camera->SetPosition(m_Camera->GetPosition() + m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (schwi::Input::IsKeyPressed(schwi::Key::S))
			m_Camera->SetPosition(m_Camera->GetPosition() - m_CameraMoveSpeed * ts * m_Camera->GetFrontVector());
		if (schwi::Input::IsKeyPressed(schwi::Key::A))
			m_Camera->SetYaw(m_Camera->GetYaw() - m_CameraRotationSpeed * ts);
		if (schwi::Input::IsKeyPressed(schwi::Key::D))
			m_Camera->SetYaw(m_Camera->GetYaw() + m_CameraRotationSpeed * ts);

		schwi::RenderCommand::SetClearColor(m_ClearColor);
		schwi::RenderCommand::Clear();

		//SW_DEBUG("Pos:{},Yaw:{}",glm::to_string(m_Camera->GetPosition()),m_Camera->GetYaw());

		schwi::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				schwi::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}
		m_Texture->Bind();
		schwi::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		schwi::Renderer::EndScene();
	}

	void OnEvent(schwi::Event& event) override
	{
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
	std::shared_ptr<schwi::Shader> m_Shader;
	std::shared_ptr<schwi::VertexArray> m_VertexArray;

	glm::vec3 m_SquareColor{ 0.2, 0.3, 0.8 };
	std::shared_ptr<schwi::Shader> m_FlatColorShader, m_TextureShader;
	std::shared_ptr<schwi::VertexArray> m_SquareVA;
	std::shared_ptr<schwi::Texture2D> m_Texture;

	std::shared_ptr<schwi::Camera> m_Camera;
	float m_CameraMoveSpeed = 5.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec4 m_ClearColor{ 102.f / 255.f, 204.f / 255.f, 1.f, 1.f };
};

class Sandbox : public schwi::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());
	}

	~Sandbox()
	{
	}
};

schwi::Application* schwi::CreateApplication()
{
	return new Sandbox();
}

int main()
{
	schwi::Log::Init();
	std::cout << "Hello!" << std::endl;
	auto app = schwi::CreateApplication();
	app->Run();

	delete app;
	return 0;
}