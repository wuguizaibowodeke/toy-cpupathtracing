#include "accelerate/bound.hpp"

Bound::Bound()
    : m_leftBottom(std::numeric_limits<float>::infinity()),
      m_rightTop(-std::numeric_limits<float>::infinity())
{
}

Bound::Bound(const glm::vec3 &leftBottom, const glm::vec3 &rightTop)
    : m_leftBottom(leftBottom),
      m_rightTop(rightTop)
{
}

const glm::vec3 &Bound::getLeftBottom() const
{
    return m_leftBottom;
}

const glm::vec3 &Bound::getRightTop() const
{
    return m_rightTop;
}

void Bound::setLeftBottom(const glm::vec3 &leftBottom)
{
    m_leftBottom = leftBottom;
}

void Bound::setRightTop(const glm::vec3 &rightTop)
{
    m_rightTop = rightTop;
}

void Bound::setBounds(const glm::vec3 &leftBottom, const glm::vec3 &rightTop)

{
    m_leftBottom = leftBottom;
    m_rightTop = rightTop;
}

void Bound::expand(const glm::vec3 &point)
{
    m_leftBottom = glm::min(m_leftBottom, point);
    m_rightTop = glm::max(m_rightTop, point);
}

void Bound::expand(const Bound &other)
{
    m_leftBottom = glm::min(m_leftBottom, other.m_leftBottom);
    m_rightTop = glm::max(m_rightTop, other.m_rightTop);
}

bool Bound::isIntersect(const Bound &other) const
{
    return !(m_leftBottom.x > other.m_rightTop.x || m_rightTop.x < other.m_leftBottom.x ||
             m_leftBottom.y > other.m_rightTop.y || m_rightTop.y < other.m_leftBottom.y ||
             m_leftBottom.z > other.m_rightTop.z || m_rightTop.z < other.m_leftBottom.z);
}

bool Bound::isIntersect(const glm::vec3 &point) const
{
    return !(point.x < m_leftBottom.x || point.x > m_rightTop.x ||
             point.y < m_leftBottom.y || point.y > m_rightTop.y ||
             point.z < m_leftBottom.z || point.z > m_rightTop.z);
}

bool Bound::isIntersect(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 t1 = (m_leftBottom - ray.origin) / ray.direction;
    glm::vec3 t2 = (m_rightTop - ray.origin) / ray.direction;
    glm::vec3 t_min_vec = glm::min(t1, t2);
    glm::vec3 t_max_vec = glm::max(t1, t2);

    float near = glm::max(t_min_vec.x, glm::max(t_min_vec.y, t_min_vec.z));
    float far = glm::min(t_max_vec.x, glm::min(t_max_vec.y, t_max_vec.z));

    if (near <= t_min && far >= t_max)
    {
        return false;
    }

    near = glm::max(near, t_min);
    far = glm::min(far, t_max);

    return near <= far;
}

bool Bound::isIntersect(const Ray &ray, glm::vec3 inv_dir, float t_min, float t_max) const
{
    glm::vec3 t1 = (m_leftBottom - ray.origin) * inv_dir;
    glm::vec3 t2 = (m_rightTop - ray.origin)  * inv_dir;
    glm::vec3 t_min_vec = glm::min(t1, t2);
    glm::vec3 t_max_vec = glm::max(t1, t2);

    float near = glm::max(t_min_vec.x, glm::max(t_min_vec.y, t_min_vec.z));
    float far = glm::min(t_max_vec.x, glm::min(t_max_vec.y, t_max_vec.z));

    if (near <= t_min && far >= t_max)
    {
        return false;
    }

    near = glm::max(near, t_min);
    far = glm::min(far, t_max);

    return near <= far;
}

glm::vec3 Bound::diagonal() const
{
    return m_rightTop - m_leftBottom;
}

float Bound::area() const
{
    auto diag = diagonal();
    return 2.0f * (diag.x * diag.y + diag.x * diag.z + diag.y * diag.z);
}

glm::vec3 Bound::getCorner(size_t index) const
{
    auto corner = m_rightTop;
    if ((index & 0b1) == 0)
    {
        corner.x = m_leftBottom.x;
    }
    if ((index & 0b10) == 0)
    {
        corner.y = m_leftBottom.y;
    }
    if ((index & 0b100) == 0)
    {
        corner.z = m_leftBottom.z;
    }
    return corner;
}

bool Bound::isValid() const
{
    return m_leftBottom.x <= m_rightTop.x &&
           m_leftBottom.y <= m_rightTop.y &&
           m_leftBottom.z <= m_rightTop.z;
}
