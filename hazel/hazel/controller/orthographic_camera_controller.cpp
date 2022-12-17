#include "orthographic_camera_controller.h"

#include <functional>

#include "hazel/core/input.h"
#include "hazel/core/key_code.h"

#include "hzpch.h"

namespace Hazel
{
    OrthographicCameraController::OrthographicCameraController(const float aspectRatio, const bool rotation)
        : m_aspect_ratio(aspectRatio),
          m_camera(-aspectRatio * m_zoom_level, aspectRatio * m_zoom_level, -m_zoom_level, m_zoom_level),
          m_rotation(rotation)
    {}
    void OrthographicCameraController::OnUpdate(Timestep timestep)
    {
        HZ_PROFILE_FUNCTION();
        auto ts = static_cast<float>(timestep);
        if (Input::IsKeyPressed(HZ_KEY_A))
        {
            m_camera_position.x -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
            m_camera_position.y -= sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
        }
        else if (Input::IsKeyPressed(HZ_KEY_D))
        {
            m_camera_position.x += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
            m_camera_position.y += sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
        }

        if (Input::IsKeyPressed(HZ_KEY_W))
        {
            m_camera_position.x += -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
            m_camera_position.y += cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
        }
        else if (Input::IsKeyPressed(HZ_KEY_S))
        {
            m_camera_position.x -= -sin(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
            m_camera_position.y -= cos(glm::radians(m_camera_rotation)) * m_camera_translation_speed * ts;
        }
        m_camera.SetPosition(m_camera_position);

        if (m_rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
            {
                m_camera_rotation += m_camera_rotation_speed * ts;
            }
            if (Input::IsKeyPressed(HZ_KEY_E))
            {
                m_camera_rotation -= m_camera_rotation_speed * ts;
            }

            // 旋转角度范围修正 -180~180
            if (m_camera_rotation > 180.0F)
            {
                m_camera_rotation -= 360.0F;
            }
            else if (m_camera_rotation <= -180.0F)
            {
                m_camera_rotation += 360.0F;
            }

            m_camera.SetRotation(m_camera_rotation);
        }

        m_camera_translation_speed = m_zoom_level;
    }

    void OrthographicCameraController::OnEvent(Event& event)
    {
        HZ_PROFILE_FUNCTION();
        EventDispatcher dispatcher(event);
        // TODO(Selene): 鼠标滚动为什么通过事件
        dispatcher.dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event)
    {
        HZ_PROFILE_FUNCTION();
        m_zoom_level -= event.GetYOffset() * 0.25F;
        m_zoom_level = std::max(m_zoom_level, 0.25F);
        m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
                               m_zoom_level);
        return false;
    }
    /// <summary>
    /// 重新计算宽高比
    /// </summary>
    /// <param name="e"></param>
    /// <returns></returns>
    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& event)
    {
        HZ_PROFILE_FUNCTION();
        m_aspect_ratio = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());
        m_camera.SetProjection(-m_aspect_ratio * m_zoom_level, m_aspect_ratio * m_zoom_level, -m_zoom_level,
                               m_zoom_level);
        return false;
    }

}  // namespace Hazel