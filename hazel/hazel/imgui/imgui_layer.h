#pragma once
#include "hazel/core/layer.h"

namespace Hazel
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;
        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        static void Begin();
        static void End();
    };
}  // namespace Hazel