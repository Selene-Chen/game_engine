
#include "hzpch.h"

#include "RenderCommand.h"

#include "Hazel/Renderer/Shader.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Hazel
{
    Scope<RenderAPI> RenderCommand::s_RendererAPI = CreateScope<OpenGLRenderAPI>();
}  // namespace Hazel