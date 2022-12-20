#include "hazel/renderer/render_command.h"

#include "platform/opengl/opengl_render_api.h"

namespace Hazel
{
    Scope<RenderAPI> RenderCommand::m_renderer_api = CreateScope<OpenGLRenderAPI>();
}  // namespace Hazel