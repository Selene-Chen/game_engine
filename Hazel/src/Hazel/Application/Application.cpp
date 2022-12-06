#include "hzpch.h"
#include <glad/glad.h>
#include "Application.h"
#include "Hazel/Renderer/Renderer.h"

namespace Hazel
{
    Application*  Application::s_instance = nullptr;
    static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
    {
        switch (type)
        {
            case ShaderDataType::Float: return GL_FLOAT;
            case ShaderDataType::Float2: return GL_FLOAT;
            case ShaderDataType::Float3: return GL_FLOAT;
            case ShaderDataType::Float4: return GL_FLOAT;
            case ShaderDataType::Mat3: return GL_FLOAT;
            case ShaderDataType::Mat4: return GL_FLOAT;
            case ShaderDataType::Int: return GL_INT;
            case ShaderDataType::Int2: return GL_INT;
            case ShaderDataType::Int3: return GL_INT;
            case ShaderDataType::Int4: return GL_INT;
            case ShaderDataType::Bool: return GL_BOOL;
        }
        HZ_CORE_ASSERT(false, "Unknow ShaderDataType!");
        return 0;
    }
    //窗口默认1280:720=16:9，正交相机必须设置成相同比例图形才不会变形
    Application::Application() : m_Camera(-1.6f,1.6f, -0.9f, 0.9f)
    {
        HZ_CORE_ASSERT(!s_instance, "Application aready exists!")
        s_instance = this;
        m_Window   = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(BIND_HZ_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushLayer(m_ImGuiLayer);

        float vertices[3 * 7] = {
            -0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f, // left
            0.5f,  -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f, // right
            0.0f,  0.5f,  0.0f, 0.8f, 0.8f, 0.2f, 1.0f  // top
        };
        unsigned int                  indices[3] = {0, 1, 2};
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer>  m_IndexBuffer;

        // 1.vertexbuffer
        m_VertexBuffer.reset(VertexBuffer::Create(vertices, sizeof(vertices)));
        BufferLayout layout = {
            {ShaderDataType::Float3, "a_Position"},
            {ShaderDataType::Float4, "a_Color"},
        };
        m_VertexBuffer->SetLayout(layout);

        // 2.indexbuffer
        m_IndexBuffer.reset(IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));

        // 3.vertexarray
        m_VertexArray.reset(VertexArray::Create());
        m_VertexArray->AddVertexBuffer(m_VertexBuffer);
        m_VertexArray->SetIndexBuffer(m_IndexBuffer);

        // 4.shader
        std::string vertextSrc  = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
            
            uniform mat4 u_ViewProjection;

			out vec3 v_Position;
			out vec4 v_Color;

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
        m_Shader.reset(new Shader(vertextSrc, fragmentSrc));

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
        unsigned int                  Squareindices[6] = {0, 1, 2, 2, 3, 0};
        std::shared_ptr<IndexBuffer>  m_SquareIndexBuffer;
        std::shared_ptr<VertexBuffer> m_SquareVertexBuffer;

        // 1.vertexbuffer
        m_SquareVertexBuffer.reset(VertexBuffer::Create(Squarevertices, sizeof(Squarevertices)));
        BufferLayout Squarelayout = {
            {ShaderDataType::Float3, "a_Position"},
        };
        m_SquareVertexBuffer->SetLayout(Squarelayout);
        // 2.indexbuffer
        m_SquareIndexBuffer.reset(IndexBuffer::Create(Squareindices, sizeof(Squareindices) / sizeof(uint32_t)));
        // 3.vertexarray
        m_SquareVertexArray.reset(VertexArray::Create());
        m_SquareVertexArray->AddVertexBuffer(m_SquareVertexBuffer);
        m_SquareVertexArray->SetIndexBuffer(m_SquareIndexBuffer);
        // 4.shader
        std::string SquarevertextSrc  = R"(
			#version 330 core

			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;

            uniform mat4 u_ViewProjection;
		
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * vec4(a_Position, 1.0);
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
        m_SquareShader.reset(new Shader(SquarevertextSrc, SquarefragmentSrc));
    }

    void Application::OnEvent(Event& e)
    {
        // 输出当前事件信息
        // HZ_CORE_TRACE("{0}", e.ToString());
        // 设置事件调度处理函数，这里是关闭窗口就结束程序
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_HZ_EVENT_FN(Application::OnWindowClose));
        // 处理层事件
        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    void Application::Run()
    {
        while (m_Running)
        {
            // TODO:清理屏
            RenderCommand::SetClearColor({0.1f, 0.1f, 0.1f, 1});
            RenderCommand::Clear();

            // TODO:开始绘制场景
            Renderer::BeginScene(m_Camera);

            // TODO:绘制1
            Renderer::Submit(m_SquareShader, m_SquareVertexArray);

            // TODO:绘制2
            Renderer::Submit(m_Shader, m_VertexArray);
            // TODO:结束绘制场景
            Renderer::EndScene();

            //// 正方形
            // m_SquareShader->Bind();
            // m_SquareVertexArray->Bind();
            // glDrawElements(GL_TRIANGLES, m_SquareVertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT,
            // nullptr);

            //// 三角形
            // m_Shader->Bind();
            // m_VertexArray->Bind();
            // glDrawElements(GL_TRIANGLES, m_VertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);

            for (Layer* layer : m_LayerStack)
                layer->OnUpdate();

            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
                layer->OnImGuiRender();
            m_ImGuiLayer->End();

            // auto [x, y] = Input::GetMousePosition();
            // HZ_CORE_TRACE("{0},{1}", x, y);

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer) { m_LayerStack.PushLayer(layer); }

    void Application::PushOverlay(Layer* overlay) { m_LayerStack.PushOverlay(overlay); }
} // namespace Hazel