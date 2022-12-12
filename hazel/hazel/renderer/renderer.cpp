#include "hazel/renderer/renderer.h"

#include "hazel/renderer/render_command.h"
#include "hazel/renderer/renderer2d.h"
#include "platform/opengl/opengl_shader.h"

namespace Hazel
{
    Scope<Renderer::SceneData> Renderer::m_scene_data = CreateScope<SceneData>();

    void Renderer::Init()
    {
        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height) { RenderCommand ::SetViewPort(0, 0, width, height); }

    void Renderer::BeginScene(OrthographicCamera& camera)
    {
        m_scene_data->ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {}

    void Renderer::Submit(Ref<Shader>& shader, Ref<VertexArray>& vertex_array, const glm::mat4& transform)
    {
        shader->Bind();
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_ViewProjection",m_scene_data->ViewProjectionMatrix);
        std::dynamic_pointer_cast<OpenGLShader>(shader)->UploadUniformMat4("u_Transform", transform);

        vertex_array->Bind();
        RenderCommand::DrawIndexed(vertex_array);
    }
}  // namespace Hazel