/*
 * @Author: Selene 339698418@qq.com
 * @Date: 2022-12-03 20:11:05
 * @LastEditors: Selene 339698418@qq.com
 * @LastEditTime: 2022-12-19 10:31:39
 * @FilePath: \game_engine\hazel\hazel\core\layer_stack.cpp
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置:
 * https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
            std::default_delete<Layer>();
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