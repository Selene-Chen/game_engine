#pragma once
#include "RenderCommand.h"

namespace Hazel
{
    class Renderer
    {
    public:
        inline static RenderAPI::API GetAPI() { return RenderAPI::GetAPI(); }
        static void                  BeginScene(OrthographicCamera& camera);
        static void                  EndScene();
        static void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray);

    private:
        struct SceneData
        {
            glm::mat4 ViewProjectionMatrix;
        };
        static SceneData* m_SceneData;
    };

} // namespace Hazel
