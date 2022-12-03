#pragma once
#include "Layer.h"

namespace Hazel
{
	class LayerStack
	{
	public:
		LayerStack();
		~LayerStack();
		//普通层，push到列表前半部分，先处理
		void PushLayer(Layer* layer);
		void PopLayer(Layer* layer);
		//覆盖层,push到列表后半部分，后处理
		void PushOverlay(Layer* overlay);
		void PopOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
		std::vector<Layer*>::iterator end() { return m_layers.end(); }
	private:
		std::vector<Layer*> m_layers;
		std::vector<Layer*>::iterator m_layersInsert;
	};
}