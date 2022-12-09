#pragma once
#include "RenderCommand.h"
#include "OrthographicCamera.h"
#include "Shader.h"
namespace Hazel
{
    class Renderer
    {
    public:
        Renderer()  = default;
        ~Renderer() = default;

    public:
        inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }

    public:
        static void Init();
        static void OnWindowResize(uint32_t width,uint32_t height);
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void Submit(const Ref<Shader>&      shader,
                           const Ref<VertexArray>& vertexArray,
                           const glm::mat4&        transform = glm::mat4(1.0f));

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static Scope<SceneData> s_SceneData;
    };

} // namespace Hazel
