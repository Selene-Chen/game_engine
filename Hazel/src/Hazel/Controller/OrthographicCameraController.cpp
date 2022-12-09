#include "hzpch.h"
#include "OrthographicCameraController.h"
#include "Hazel/Core/Input.h"
#include "Hazel/Core/KeyCode.h"

namespace Hazel
{
    /// <summary>
    /// zoom控制缩放等级，默认为1，值越大放大，值越小缩小
    /// </summary>
    /// <param name="aspectRatio"></param>
    /// <param name="rotation"></param>
    OrthographicCameraController::OrthographicCameraController(float aspectRatio, bool rotation /*= false*/) :
        m_AspectRatio(aspectRatio),
        m_Camera(-aspectRatio * m_ZoomLevel, aspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel), m_Rotation(rotation)
    {}

    void OrthographicCameraController::OnUpdate(Timestep timestep)
    {
        if (Hazel::Input::IsKeyPressed(HZ_KEY_A))
        {
            m_CameraPosition.x -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y -= sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_D))
        {
            m_CameraPosition.x += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y += sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }

        if (Hazel::Input::IsKeyPressed(HZ_KEY_W))
        {
            m_CameraPosition.x += -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y += cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        else if (Hazel::Input::IsKeyPressed(HZ_KEY_S))
        {
            m_CameraPosition.x -= -sin(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
            m_CameraPosition.y -= cos(glm::radians(m_CameraRotation)) * m_CameraTranslationSpeed * timestep;
        }
        m_Camera.SetPosition(m_CameraPosition);

        if (m_Rotation)
        {
            if (Input::IsKeyPressed(HZ_KEY_Q))
                m_CameraRotation += m_CameraRotationSpeed * timestep;
            if (Input::IsKeyPressed(HZ_KEY_E))
                m_CameraRotation -= m_CameraRotationSpeed * timestep;
               

            // 旋转角度范围修正 -180~180
            if (m_CameraRotation > 180.0f)
                m_CameraRotation -= 360.0f;
            else if (m_CameraRotation <= -180.0f)
                m_CameraRotation += 360.0f;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_CameraTranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<MouseScrolledEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }
    /// <summary>
    /// 滚轮向上为负，向下为正
    /// 向上m_ZoomLevel+，无上限
    /// 向下m_ZoomLevel-，最小值0.25
    /// </summary>
    /// <param name="e"></param>
    /// <returns></returns>
    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }
    /// <summary>
    /// 重新计算宽高比
    /// </summary>
    /// <param name="e"></param>
    /// <returns></returns>
    bool OrthographicCameraController::OnWindowResized(WindowResizeEvent& e)
    {
        m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

} // namespace Hazel