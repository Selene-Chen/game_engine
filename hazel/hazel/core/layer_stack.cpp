#include "layer_stack.h"

#include <vector>

#include "hzpch.h"

namespace Hazel
{
    LayerStack::~LayerStack()
    {
        for (const Ref<Layer>& layer : m_layers)
        {
            layer->OnDetach();
            delete layer.get();
        }
    }

    void LayerStack::PushLayer(const Ref<Layer>& layer)
    {
        m_layers.emplace(m_layers.begin() + m_layer_insert_index, layer);
        m_layer_insert_index++;
        layer->OnAttach();
    }

    void LayerStack::PopLayer(const Ref<Layer>& layer)
    {
        auto it = std::find(m_layers.begin(), m_layers.begin() + m_layer_insert_index, layer);
        if (it != m_layers.begin() + m_layer_insert_index)
        {
            layer->OnDetach();
            m_layers.erase(it);
            m_layer_insert_index--;
        }
    }

    void LayerStack::PushOverlay(const Ref<Layer>& overlay)
    {
        m_layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopOverlay(const Ref<Layer>& overlay)
    {
        auto it = std::find(m_layers.begin() + m_layer_insert_index, m_layers.end(), overlay);
        if (it != m_layers.end())
        {
            overlay->OnDetach();
            m_layers.erase(it);
        }
    }
}  // namespace Hazel