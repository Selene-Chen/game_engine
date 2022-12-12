#pragma once
#include "hazel/renderer/orthographic_camera.h"
#include "hazel/renderer/render_api.h"
#include "hazel/renderer/shader.h"

namespace Hazel
{
    class Renderer
    {
    public:
        Renderer() = default;
        ~Renderer() = default;

        static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

        static void Init();
        static void OnWindowResize(uint32_t width, uint32_t height);
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void Submit(Ref<Shader>& shader, Ref<VertexArray>& vertex_array,
                           const glm::mat4& transform = glm::mat4(1.0F));

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };

        static Scope<SceneData> m_scene_data;
    };

}  // namespace Hazel