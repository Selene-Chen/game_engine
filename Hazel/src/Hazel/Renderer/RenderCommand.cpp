
#include "hzpch.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "Platform/OpenGL/OpenGLRenderAPI.h"

namespace Hazel
{
    RenderAPI* RenderCommand::s_RendererAPI = new OpenGLRenderAPI;
} // namespace Hazel