#include "hazel/renderer/renderer_command.h"

#include "platform/opengl/opengl_renderer_api.h"

namespace Hazel
{
    Scope<RendererAPI> RendererCommand::m_renderer_api = CreateScope<OpenGLRendererAPI>();
}  // namespace Hazel