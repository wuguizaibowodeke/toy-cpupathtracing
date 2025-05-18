#include "geometry/plane.hpp"

Plane::Plane(const glm::vec3 &point, const glm::vec3 &normal)
    : m_normal(normal), m_point(point)
{
    m_normal = glm::normalize(m_normal);
}

std::optional<RayHitInfo> Plane::intersect(const Ray &ray, float t_min, float t_max) const
{
    float denominator = glm::dot(m_normal, ray.direction);
    if (std::abs(denominator) < 1e-6)
    {
        return {}; // 平行于平面
    }

    float t = glm::dot(m_point - ray.origin, m_normal) / denominator;
    if (t < t_min || t > t_max)
    {
        return {}; // 不在有效范围内
    }

    glm::vec3 hit_point = ray.hit(t);
    return RayHitInfo{t, m_normal, hit_point};
}