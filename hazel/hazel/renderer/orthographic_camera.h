#pragma once
#include <glm/glm.hpp>

namespace Hazel
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera() = default;

        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(glm::vec3& position)
        {
            m_position = position;
            RecalculateViewMatrix();
        }
        void SetRotation(const float rotation)
        {
            m_rotation = rotation;
            RecalculateViewMatrix();
        }

        [[nodiscard]] float GetRotion() const { return m_rotation; }
        [[nodiscard]] glm::vec3 GetPosition() const { return m_position; }
        [[nodiscard]] const glm::mat4& GetProjectionMatrix() const { return m_projection_matrix; }
        [[nodiscard]] const glm::mat4& GetViewMatrix() const { return m_view_matrix; }
        [[nodiscard]] const glm::mat4& GetViewProjectionMatrix() const { return m_view_projection_matrix; }

    private:
        void RecalculateViewMatrix();

        glm::mat4 m_projection_matrix;
        glm::mat4 m_view_matrix;
        glm::mat4 m_view_projection_matrix;
        glm::vec3 m_position = {0.0F, 0.0F, 0.0F};
        float m_rotation = 0.0F;
    };
}  // namespace Hazel