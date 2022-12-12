#pragma once
#include <vector>

#include "hazel/core/layer.h"

namespace Hazel
{
    class LayerStack
    {
    public:
        ~LayerStack();

        void PushLayer(const Ref<Layer>& layer);
        void PopLayer(const Ref<Layer>& layer);

        void PushOverlay(const Ref<Layer>& overlay);
        void PopOverlay(const Ref<Layer>& overlay);

        std::vector<Ref<Layer>>::iterator begin() { return m_layers.begin(); }
        std::vector<Ref<Layer>>::iterator end() { return m_layers.end(); }
        [[nodiscard]] std::vector<Ref<Layer>>::const_iterator begin() const { return m_layers.begin(); }
        [[nodiscard]] std::vector<Ref<Layer>>::const_iterator end() const { return m_layers.end(); }

        std::vector<Ref<Layer>>::reverse_iterator rbegin() { return m_layers.rbegin(); }
        std::vector<Ref<Layer>>::reverse_iterator rend() { return m_layers.rend(); }
        [[nodiscard]] std::vector<Ref<Layer>>::const_reverse_iterator rbegin() const { return m_layers.rbegin(); }
        [[nodiscard]] std::vector<Ref<Layer>>::const_reverse_iterator rend() const { return m_layers.rend(); }

    private:
        std::vector<Ref<Layer>> m_layers;
        unsigned int m_layer_insert_index = 0;
    };
}  // namespace Hazel