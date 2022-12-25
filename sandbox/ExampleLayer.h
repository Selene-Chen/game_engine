#include <glm/gtc/matrix_transform.hpp>
#include <hazel.h>
#include <imgui.h>

// 窗口默认1280:720=16:9，正交相机必须设置成相同比例图形才不会变形
class ExampleLayer : public Hazel::Layer
{
public:
    ExampleLayer()
        : Layer("Example"), m_vertex_array(Hazel::VertexArray::Create()), m_camera_controller(1280.0F / 720.0F, true)
    {
        std::array<float, (static_cast<size_t>(3 * 7))> vertices = {
            -0.5F, -0.5F, 0.0F, 0.8F, 0.2F, 0.8F, 1.0F,  // leFt
            0.5F,  -0.5F, 0.0F, 0.2F, 0.3F, 0.8F, 1.0F,  // right
            0.0F,  0.5F,  0.0F, 0.8F, 0.8F, 0.2F, 1.0F   // top
        };
        std::array<uint32_t, 3> indices = {0, 1, 2};

        // 1.vertexbuffer
        const Hazel::Ref<Hazel::VertexBuffer> vertex_buffer =
            Hazel::VertexBuffer::Create(vertices.data(), sizeof(vertices));
        const Hazel::BufferLayout layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float4,    "a_Color"},
        };
        vertex_buffer->SetLayout(layout);

        // 2.indexbuffer
        const Hazel::Ref<Hazel::IndexBuffer> index_buffer =
            Hazel::IndexBuffer::Create(indices.data(), sizeof(indices) / sizeof(uint32_t));

        // 3.vertexarray
        m_vertex_array->AddVertexBuffer(vertex_buffer);
        m_vertex_array->SetIndexBuffer(index_buffer);

        // 4.shader
        const std::string vertext_src  = R"(
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
        const std::string fragment_src = R"(
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

        m_shader = Hazel::Shader::Create("Shader", vertext_src, fragment_src);

        //--------正方形（Square）------------------------------------
        std::array<float, static_cast<size_t>(5 * 4)> square_vertices = {
            // 位置              // 纹理坐标
            -0.5F, -0.5F, 0.0F, 0.0F, 0.0F,  // leFt down
            0.5F,  -0.5F, 0.0F, 1.0F, 0.0F,  // right down
            0.5F,  0.5F,  0.0F, 1.0F, 1.0F,  // right top
            -0.5F, 0.5F,  0.0F, 0.0F, 1.0F   // left top
        };
        std::array<uint32_t, 6> square_indices = {0, 1, 2, 2, 3, 0};

        // 1.vertexbuffer
        const Hazel::Ref<Hazel::VertexBuffer> square_vertex_buffer =
            Hazel::VertexBuffer::Create(square_vertices.data(), sizeof(square_vertices));
        const Hazel::BufferLayout square_layout = {
            {Hazel::ShaderDataType::Float3, "a_Position"},
            {Hazel::ShaderDataType::Float2, "a_TexCoore"},
        };
        square_vertex_buffer->SetLayout(square_layout);
        // 2.indexbuffer
        const Hazel::Ref<Hazel::IndexBuffer> square_index_buffer =
            Hazel::IndexBuffer::Create(square_indices.data(), sizeof(square_indices) / sizeof(uint32_t));
        // 3.vertexarray
        m_square_vertex_array = Hazel::VertexArray::Create();
        m_square_vertex_array->AddVertexBuffer(square_vertex_buffer);
        m_square_vertex_array->SetIndexBuffer(square_index_buffer);
        // 4.shader
        const std::string square_vertext_src  = R"(
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
        const std::string square_fragment_src = R"(
			#version 330 core
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
                        uniform vec3 u_Color;
			void main()
			{
				color = vec4(u_Color,1.0);
			}
		)";

        m_square_shader = Hazel::Shader::Create("SquareShader", square_vertext_src, square_fragment_src);

        // 加载 texture shader
        // m_TextureShader = Hazel::Shader::Create("assets/shader/texture.glsl");
        const auto texture_shader = m_shader_library.Load("assets/shader/texture.glsl");
        // 加载纹理
        m_texture      = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
        m_logo_texture = Hazel::Texture2D::Create("assets/textures/Logo.png");
        // 着色器设置纹理变量
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(texture_shader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(texture_shader)->UploadUniformInt("u_Texture", 0);
    }
    void OnUpdate(const Hazel::Timestep timestep) override
    {
        // HZ_TRACE("Delta time: {0}s ({1}ms)", timestep.GetSeconds(), timestep.GetMilliseconds());

        // updateCamera
        m_camera_controller.OnUpdate(timestep);

        // Render
        Hazel::RendererCommand::SetClearColor({0.1F, 0.1F, 0.1F, 1});
        Hazel::RendererCommand::Clear();

        Hazel::Renderer::BeginScene(m_camera_controller.GetCamera());

        // 正方形变换
        const glm::mat4 square_scale = glm::scale(glm::mat4(1.0F), glm::vec3(0.1F));

        // 颜色
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_square_shader)->Bind();
        std::dynamic_pointer_cast<Hazel::OpenGLShader>(m_square_shader)->UploadUniformFloat3("u_Color", m_square_color);

        for (int x = 0; x < 15; x++)
        {
            for (int y = 0; y < 15; y++)
            {
                glm::vec3 pos(static_cast<float>(x) * 0.11F, static_cast<float>(y) * 0.11F, 0.0F);
                glm::mat4 square_transform = glm::translate(glm::mat4(1.0F), pos) * square_scale;
                Hazel::Renderer::Submit(m_square_shader, m_square_vertex_array, square_transform);
            }
        }
        auto texture_shader = m_shader_library.Get("texture");
        // 纹理绑定
        m_texture->Bind(0);
        Hazel::Renderer::Submit(texture_shader, m_square_vertex_array, glm::scale(glm::mat4(1.0F), glm::vec3(1.0F)));
        // 纹理绑定
        m_logo_texture->Bind(0);
        Hazel::Renderer::Submit(texture_shader, m_square_vertex_array, glm::scale(glm::mat4(1.0F), glm::vec3(1.0F)));

        // 绘制三角形
        //  Hazel::Renderer::Submit(m_Shader, m_VertexArray);

        // 结束绘制场景
        Hazel::Renderer::EndScene();
    }

    void OnImGuiRender() override
    {
        ImGui::Begin("Settings");
        ImGui::ColorEdit3("Square Color", glm::value_ptr(m_square_color));
        ImGui::End();
    }
    void OnEvent(Hazel::Event& event) override { m_camera_controller.OnEvent(event); }

private:
    // 三角形
    Hazel::Ref<Hazel::Shader> m_shader;
    Hazel::Ref<Hazel::VertexArray> m_vertex_array;
    // 正方形
    Hazel::Ref<Hazel::VertexArray> m_square_vertex_array;
    Hazel::Ref<Hazel::Shader> m_square_shader;
    // Texture shder
    Hazel::Ref<Hazel::Shader> m_texture_shader;
    // shderLibrary
    Hazel::ShaderLibrary m_shader_library;
    // Texture file
    Hazel::Ref<Hazel::Texture2D> m_texture;
    Hazel::Ref<Hazel::Texture2D> m_logo_texture;
    // CameraController
    Hazel::OrthographicCameraController m_camera_controller;

    // Square Transform
    glm::vec3 m_square_position = {0.0F, 0.0F, 0.0F};
    // Square Color
    glm::vec3 m_square_color = {0.0F, 0.0F, 0.0F};
};