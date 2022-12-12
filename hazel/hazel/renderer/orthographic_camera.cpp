#include "hazel/renderer/orthographic_camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Hazel
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_projection_matrix(glm::ortho(left, right, bottom, top, -1.0F, 1.0F)),
          m_view_matrix(1.0F),
          m_view_projection_matrix(m_projection_matrix * m_view_matrix)
    {}

    void OrthographicCamera::SetProjection(const float left, const float right, const float bottom, const float top)
    {
        m_projection_matrix = glm::ortho(left, right, bottom, top, -1.0F, 1.0F);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        // 先rotate后translate
        const glm::mat4 transform = translate(glm::mat4(1.0F), m_position) *
                                    rotate(glm::mat4(1.0F), glm::radians(m_rotation), glm::vec3(0.0F, 0.0F, 1.0F));
        m_view_matrix = inverse(transform);
        m_view_projection_matrix = m_projection_matrix * m_view_matrix;
    }

}  // namespace Hazel