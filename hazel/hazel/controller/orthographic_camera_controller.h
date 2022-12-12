#pragma once
#include <glm/glm.hpp>

#include "hazel/core/timestep.h"
#include "hazel/events/application_event.h"
#include "hazel/events/mouse_event.h"
#include "hazel/renderer/orthographic_camera.h"

namespace Hazel
{
    class OrthographicCameraController
    {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool rotation = false);
        ~OrthographicCameraController() = default;

        void OnUpdate(Timestep timestep);
        void OnEvent(Event& event);
        OrthographicCamera& GetCamera() { return m_camera; }

    private:
        bool OnMouseScrolled(MouseScrolledEvent& event);
        bool OnWindowResized(WindowResizeEvent& event);

        float m_aspect_ratio;
        float m_zoom_level = 1.0F;
        OrthographicCamera m_camera;
        bool m_rotation;

        // 摄像机属性
        glm::vec3 m_camera_position = {0.0F, 0.0F, 0.0F};
        float m_camera_rotation = 0.0F;
        float m_camera_translation_speed = 5.0F, m_camera_rotation_speed = 180.0F;
    };
}  // namespace Hazel