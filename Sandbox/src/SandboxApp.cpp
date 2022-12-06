#include <Hazel.h>
#include <glm/gtc/matrix_transform.hpp>
// 窗口默认1280:720=16:9，正交相机必须设置成相同比例图形才不会变形
class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer() : Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // left
            0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, // right
            0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f  // top
        };
        unsigned int                         indices[3] = {0, 1, 2};
        std::shared_ptr<Hazel::VertexBuffer> m_VertexBuffer;
        std::shared_ptr<Hazel::IndexBuffer>  m_IndexBuffer;

        // 1.vertexbuffer
        m_VertexBuffer.reset(Hazel::VertexBuffer::Create(vertices, sizeof(vertices)));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);

        // 2.indexbuffer
        m_IndexBuffer.reset(Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        // 3.vertexarray
        m_VertexArray.reset(Hazel::VertexArray::Create());
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        // 4.shader
        std::string vertextSrc  = R"(
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
				color = vec4(v_Position * 0.5 + 0.5,1.0);
				color = v_Color;
			}
		)";
        m_Shader.reset(new Hazel::Shader(vertextSrc, fragmentSrc));

        //--------正方形（Square）------------------------------------
        float Squarevertices[3 * 4] = {
            -0.5f,
            -0.5f,
            0.0f, // left down
            0.5f,
            -0.5f,
            0.0f, // right down
            0.5f,
            0.5f,
            0.0f, // right top
            -0.5f,
            0.5f,
            0.0f // left top
        };
        unsigned int                         Squareindices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<Hazel::IndexBuffer>  m_SquareIndexBuffer;
        std::shared_ptr<Hazel::VertexBuffer> m_SquareVertexBuffer;

        // 1.vertexbuffer
        m_SquareVertexBuffer.reset(Hazel::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));
        Hazel::BufferLayout Squarelayout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
        };
        m_SquareVertexBuffer->SetLayout(Squarelayout);
        // 2.indexbuffer
        m_SquareIndexBuffer.reset(Hazel::IndexBuffer::Create(Squareindices, sizeof(Squareindices) / sizeof(uint32_t)));
        // 3.vertexarray
        m_SquareVertexArray.reset(Hazel::VertexArray::Create());
        m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
        m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);
        // 4.shader
        std::string SquarevertextSrc  = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

            uniform mat4 u_ViewProjection;
            uniform mat4 u_Transform;		    

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
			}
		)";
        std::string SquarefragmentSrc = R"(
			#version 330 core

			layout(location = 0) out vec4 color;
			
			in vec3 v_Position;

			void main()
			{
				color = vec4(0.2,0.3,0.8,1.0);
			}
		)";
        m_SquareShader.reset(new Hazel::Shader(SquarevertextSrc, SquarefragmentSrc));
    }
    void OnUpdate(Hazel::Timestep timestep) override
    {
        // HZ_TRACE("Delta time: {0}s ({1}ms)", timestep.GetSeconds(), timestep.GetMilliseconds());

        // Camera
        if (Hazel::Input::IsKeyPressed(HZ_KEY_LEFT))
            m_CameraPosition.x -= m_CameraMoveSpeed * timestep;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_RIGHT))
            m_CameraPosition.x += m_CameraMoveSpeed * timestep;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_UP))
            m_CameraPosition.y += m_CameraMoveSpeed * timestep;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_DOWN))
            m_CameraPosition.y -= m_CameraMoveSpeed * timestep;

        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
            m_CameraRotation += m_CameraRotationSpeed * timestep;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
            m_CameraRotation -= m_CameraRotationSpeed * timestep;

        // transform
        if (Hazel::Input::IsKeyPressed(HZ_KEY_J))
            m_SquarePosition.x -= m_SqauareMoveSpeed * timestep;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_L))
            m_SquarePosition.x += m_SqauareMoveSpeed * timestep;
        if (Hazel::Input::IsKeyPressed(HZ_KEY_I))
            m_SquarePosition.y += m_SqauareMoveSpeed * timestep;
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_K))
            m_SquarePosition.y -= m_SqauareMoveSpeed * timestep;

        // TODO:清理屏
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();

        // 移动、旋转相机
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);
        glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), m_SquarePosition);

        // TODO:开始绘制场景
        Hazel::Renderer::BeginScene(m_Camera);

        // TODO:绘制1
        Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, squareTransform);

        // TODO:绘制2
        Hazel::Renderer::Submit(m_Shader, m_VertexArray);
        // TODO:结束绘制场景
        Hazel::Renderer::EndScene();
    }
    void OnEvent(Hazel::Event& event) override {}

private:
    // 三角形
    std::shared_ptr<Hazel::Shader>      m_Shader;
    std::shared_ptr<Hazel::VertexArray> m_VertexArray;
    // 正方形
    std::shared_ptr<Hazel::VertexArray> m_SquareVertexArray;
    std::shared_ptr<Hazel::Shader>      m_SquareShader;
    Hazel::OrthographicCamera           m_Camera;
    //Camera
    float                               m_CameraRotation      = 0.0f;
    glm::vec3                           m_CameraPosition      = {0.0f, 0.0f, 0.0f};
    //Camera speed
    float                               m_CameraMoveSpeed     = 1.0f;
    float                               m_CameraRotationSpeed = 180.0f;
    //Square Transform
    glm::vec3                           m_SquarePosition      = {0.0f, 0.0f, 0.0f};
    float                               m_SqauareMoveSpeed    = 1.0f;
};
class Sandbox : public Hazel::Application
{
public:
    Sandbox() { PushLayer(new ExampleLayer()); }
    ~Sandbox() {}
};
Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }