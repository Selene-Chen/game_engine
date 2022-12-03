#pragma once
#include "Hazel/Layer/Layer.h"

namespace Hazel{
	class ImguiLayer:public Layer
	{
	public:
		ImguiLayer();
		~ImguiLayer();
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void OnEvent(Event& event) override;
	private:
		float m_time = 0.0;
	};
}