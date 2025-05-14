#pragma once

#include <glm/glm.hpp>
#include "camera/ray.hpp"

class Bound
{
public:
    Bound() : m_leftBottom(0.0f), m_rightTop(0.0f)
    {
    }

    Bound(const glm::vec3 &leftBottom, const glm::vec3 &rightTop)
        : m_leftBottom(leftBottom), m_rightTop(rightTop)
    {       
    }

    const glm::vec3 &getLeftBottom() const { return m_leftBottom; }
    
    const glm::vec3 &getRightTop() const { return m_rightTop; }

    void setLeftBottom(const glm::vec3 &leftBottom) { m_leftBottom = leftBottom; }

    void setRightTop(const glm::vec3 &rightTop) { m_rightTop = rightTop; }

    void setBounds(const glm::vec3 &leftBottom, const glm::vec3 &rightTop)
    {
        m_leftBottom = leftBottom;
        m_rightTop = rightTop;
    }

    void expand(const glm::vec3 &point)
    {
        m_leftBottom = glm::min(m_leftBottom, point);
        m_rightTop = glm::max(m_rightTop, point);
    }

    bool isIntersect(const Bound &other) const
    {
        return !(m_leftBottom.x > other.m_rightTop.x || m_rightTop.x < other.m_leftBottom.x ||
                 m_leftBottom.y > other.m_rightTop.y || m_rightTop.y < other.m_leftBottom.y ||
                 m_leftBottom.z > other.m_rightTop.z || m_rightTop.z < other.m_leftBottom.z);
    }

    bool isIntersect(const glm::vec3 &point) const
    {
        return !(point.x < m_leftBottom.x || point.x > m_rightTop.x ||
                 point.y < m_leftBottom.y || point.y > m_rightTop.y ||
                 point.z < m_leftBottom.z || point.z > m_rightTop.z);
    }

    bool isIntersect(const Ray &ray, float t_min, float t_max) const
    {
        glm::vec3 t1 = (m_leftBottom - ray.origin) / ray.direction;
        glm::vec3 t2 = (m_rightTop - ray.origin) / ray.direction;
        glm::vec3 t_min_vec = glm::min(t1, t2);
        glm::vec3 t_max_vec = glm::max(t1, t2);

        float near = glm::max(t_min_vec.x, glm::max(t_min_vec.y, t_min_vec.z));
        float far = glm::min(t_max_vec.x, glm::min(t_max_vec.y, t_max_vec.z));

        near = glm::max(near, t_min);
        far = glm::min(far, t_max);

        return near <= far;
    }

private:
    glm::vec3 m_leftBottom;
    glm::vec3 m_rightTop;
};