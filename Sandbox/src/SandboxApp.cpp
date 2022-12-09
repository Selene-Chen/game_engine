#include <Hazel.h>
#include <glm/gtc/matrix_transform.hpp>
#include "Platform/OpenGL/OpenGLShader.h"
#include "Platform/OpenGL/OpenGLTexturer.h"
#include "imgui.h"

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
        unsigned int                    indices[3] = {0, 1, 2};
        Hazel::Ref<Hazel::VertexBuffer> m_VertexBuffer;
        Hazel::Ref<Hazel::IndexBuffer>  m_IndexBuffer;

        // 1.vertexbuffer
        m_VertexBuffer             = Hazel::VertexBuffer::Create(vertices, sizeof(vertices));
        Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);

        // 2.indexbuffer
        m_IndexBuffer = Hazel::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

        // 3.vertexarray
        m_VertexArray = Hazel::VertexArray::Create();
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

        m_Shader = Hazel::Shader::Create("Shader", vertextSrc, fragmentSrc);

        //--------正方形（Square）------------------------------------
        float Squarevertices[5 * 4] = {
            // 位置              // 纹理坐标
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // left down
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f, // right down
            0.5f,  0.5f,  0.0f, 1.0f, 1.0f, // right top
            -0.5f, 0.5f,  0.0f, 0.0f, 1.0f  // left top
        };
        unsigned int                    Squareindices[6] = {0, 1, 2, 2, 3, 0};
        Hazel::Ref<Hazel::IndexBuffer>  m_SquareIndexBuffer;
        Hazel::Ref<Hazel::VertexBuffer> m_SquareVertexBuffer;

        // 1.vertexbuffer
        m_SquareVertexBuffer             = Hazel::VertexBuffer::Create(Squarevertices, sizeof(Squarevertices));
        Hazel::BufferLayout Squarelayout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float2, "a_TexCoore"},
        };
        m_SquareVertexBuffer->SetLayout(Squarelayout);
        // 2.indexbuffer
        m_SquareIndexBuffer = Hazel::IndexBuffer::Create(Squareindices, sizeof(Squareindices) / sizeof(uint32_t));
        // 3.vertexarray
        m_SquareVertexArray = Hazel::VertexArray::Create();
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

            uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color,1.0);
			}
		)";

        m_SquareShader = Hazel::Shader::Create("SquareShader", SquarevertextSrc, SquarefragmentSrc);

        // 加载 texture shader
        // m_TextureShader = Hazel::Shader::Create("assets/shader/texture.glsl");
        auto textureShader = m_ShaderLibrary.Load("assets/shader/texture.glsl");
        // 加载纹理
        m_Texture     = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        m_LogoTexture = Hazel::Texture2D::Create("assets/textures/Logo.png");
        // 着色器设置纹理变量
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(textureShader)->UploadUniformInt("u_Texture", 0);
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

        // 清理屏
        Hazel::RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
        Hazel::RenderCommand::Clear();

        // 移动、旋转相机
        m_Camera.SetPosition(m_CameraPosition);
        m_Camera.SetRotation(m_CameraRotation);

        // 开始绘制场景
        Hazel::Renderer::BeginScene(m_Camera);

        // 正方形变换
        glm::mat4 squareScale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        // 颜色
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_SquareShader)->UploadUniformFloat3("u_Color", m_SquareColor);

        for (int x = 0; x < 15; x++)
        {
            for (int y = 0; y < 15; y++)
            {
                glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
                glm::mat4 squareTransform = glm::translate(glm::mat4(1.0f), pos) * squareScale;
                Hazel::Renderer::Submit(m_SquareShader, m_SquareVertexArray, squareTransform);
            }
        }
        auto textureShader = m_ShaderLibrary.Get("texture");
        // 纹理绑定
        m_Texture->Bind();
        Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));
        // 纹理绑定
        m_LogoTexture->Bind();
        Hazel::Renderer::Submit(textureShader, m_SquareVertexArray, glm::scale(glm::mat4(1.0f), glm::vec3(1.0f)));

        // 绘制三角形
        //  Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        // TODO:结束绘制场景
        Hazel::Renderer::EndScene();
    }
    virtual void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
        ImGui::End();
    }
    void OnEvent(Hazel::Event& event) override {}

private:
    // 三角形
    Hazel::Ref<Hazel::Shader>      m_Shader;
    Hazel::Ref<Hazel::VertexArray> m_VertexArray;
    // 正方形
    Hazel::Ref<Hazel::VertexArray> m_SquareVertexArray;
    Hazel::Ref<Hazel::Shader>      m_SquareShader;
    Hazel::OrthographicCamera      m_Camera;
    // Texture shder
    Hazel::Ref<Hazel::Shader>        m_TextureShader;
    // shderLibrary
    Hazel::ShaderLibrary m_ShaderLibrary;
    // Texture file
    Hazel::Ref<Hazel::Texture2D> m_Texture;
    Hazel::Ref<Hazel::Texture2D> m_LogoTexture;
    // Camera
    float     m_CameraRotation = 0.0f;
    glm::vec3 m_CameraPosition = {0.0f, 0.0f, 0.0f};
    // Camera speed
    float m_CameraMoveSpeed     = 1.0f;
    float m_CameraRotationSpeed = 180.0f;
    // Square Transform
    glm::vec3 m_SquarePosition   = {0.0f, 0.0f, 0.0f};
    float     m_SqauareMoveSpeed = 1.0f;
    // Square Color
    glm::vec3 m_SquareColor = {0.0f, 0.0f, 0.0f};
};
class Sandbox : public Hazel::Application
{
public:
    Sandbox() { PushLayer(new ExampleLayer()); }
    ~Sandbox() {}
};
Hazel::Application* Hazel::CreateApplication() { return new Sandbox(); }