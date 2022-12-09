#pragma once
#include "Layer.h"

namespace Hazel
{
    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();
        // 普通层，push到列表前半部分，先处理
        void PushLayer(Layer* layer);
        void PopLayer(Layer* layer);
        // 覆盖层,push到列表后半部分，后处理
        void                          PushOverlay(Layer* overlay);
        void                          PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
        std::vector<Layer*>::iterator end() { return m_Layers.end(); }

    private:
        std::vector<Layer*> m_Layers;
        unsigned int        m_LayerInsertIndex = 0;
    };
} // namespace Hazel