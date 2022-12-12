#pragma once
#include "hazel/core/core.h"
namespace Hazel
{
    class GraphicsContext
    {
    public:
        GraphicsContext() = default;
        virtual ~GraphicsContext() = default;

        static Scope<GraphicsContext> Create(void* window);

        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}  // namespace Hazel