#pragma once
#include "Hazel/Core.h"
namespace Hazel
{
    class GraphicsContext
    {
    public:
        GraphicsContext()          = default;
        virtual ~GraphicsContext() = default;
        virtual void Init()        = 0;
        virtual void SwapBuffers() = 0;
    };
} // namespace Hazel