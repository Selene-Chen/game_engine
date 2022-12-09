
#include "hzpch.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Hazel
{
    Scope<RenderAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRenderAPI>();
} // namespace Hazel