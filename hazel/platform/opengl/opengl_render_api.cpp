
#include "platform/opengl/opengl_render_api.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Hazel
{

    void OpenGLRenderAPI::Init()
    {
        // 混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 深度测试
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRenderAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

    void OpenGLRenderAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void OpenGLRenderAPI::DrawIndexed(const Ref<VertexArray>& vertex_array)
    {
        glDrawElements(GL_TRIANGLES, static_cast<int>(vertex_array->GetIndexBuffer()->GetCount()), GL_UNSIGNED_INT,
                       nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void OpenGLRenderAPI::SetViewPort(const int x, const int y, const int width, const int height)
    {
        glViewport(x, y, width, height);
    }
}  // namespace Hazel