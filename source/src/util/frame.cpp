#include "util/frame.hpp"

Frame::Frame(const glm::vec3 &normal)
{
    m_y_axis = normal;
    glm::vec3 up = glm::abs(normal.y) < 0.99999 ? glm::vec3(0, 1, 0) : glm::vec3(0, 0, 1);
    m_x_axis = glm::normalize(glm::cross(up, normal));
    m_z_axis = glm::normalize(glm::cross(m_x_axis, m_y_axis));
}

glm::vec3 Frame::localFromWorld(const glm::vec3 &v) const
{
    return glm::normalize(glm::vec3(
        glm::dot(v, m_x_axis),
        glm::dot(v, m_y_axis),
        glm::dot(v, m_z_axis)));
}

glm::vec3 Frame::worldFromLocal(const glm::vec3 &v) const
{
    return glm::normalize(v.x * m_x_axis +
                          v.y * m_y_axis +
                          v.z * m_z_axis);
}
