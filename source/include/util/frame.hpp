#pragma once

#include <glm/glm.hpp>

class Frame
{
public:
    Frame(const glm::vec3 &normal);

    glm::vec3 localFromWorld(const glm::vec3 &v) const;

    glm::vec3 worldFromLocal(const glm::vec3 &v) const;

private:
    glm::vec3 m_x_axis;
    glm::vec3 m_y_axis;
    glm::vec3 m_z_axis;
};