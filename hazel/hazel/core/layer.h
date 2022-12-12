#pragma once
#include "hazel/core/timestep.h"
#include "hazel/events/event.h"


namespace Hazel
{
    class Layer
    {
    public:
        explicit Layer(std::string name);
        virtual ~Layer() = default;

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep timestep) {}
        virtual void OnImGuiRender() {}
        virtual void OnEvent(Event& event) {}

        [[nodiscard]] const std::string& GetName() const { return m_debug_name; }

    private:
        std::string m_debug_name;
    };
}  // namespace Hazel