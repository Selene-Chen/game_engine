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
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();
        static void Submit(const std::shared_ptr<Shader>&      shader,
                           const std::shared_ptr<VertexArray>& vertexArray,
                           const glm::mat4&                    transform = glm::mat4(1.0f));

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static SceneData* s_SceneData;
    };

} // namespace Hazel
