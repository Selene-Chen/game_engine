#pragma once
#include "Hazel/Core/Layer/Layer.h"

namespace Hazel
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;
        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();
    };
}  // namespace Hazel