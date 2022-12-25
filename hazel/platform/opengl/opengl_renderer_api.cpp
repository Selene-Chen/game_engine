
#include "platform/opengl/opengl_renderer_api.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "hzpch.h"

namespace Hazel
{

    void OpenGLRendererAPI::Init()
    {
        HZ_PROFILE_FUNCTION();
        // 混合
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        // 深度测试
        glEnable(GL_DEPTH_TEST);
    }

    void OpenGLRendererAPI::SetClearColor(const glm::vec4& color) { glClearColor(color.r, color.g, color.b, color.a); }

    void OpenGLRendererAPI::Clear() { glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); }

    void OpenGLRendererAPI::DrawIndexed(const Ref<VertexArray>& vertex_array, uint32_t index_count)
    {
        uint32_t count = index_count != 0 ? index_count : vertex_array->GetIndexBuffer()->GetCount();

        glDrawElements(GL_TRIANGLES, static_cast<int>(count), GL_UNSIGNED_INT, nullptr);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    void OpenGLRendererAPI::SetViewPort(const int x, const int y, const int width, const int height)
    {
        glViewport(x, y, width, height);
    }
}  // namespace Hazel