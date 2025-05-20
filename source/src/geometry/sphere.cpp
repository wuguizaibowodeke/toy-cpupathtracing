#include "geometry/sphere.hpp"
#include <iostream>

Sphere::Sphere(const glm::vec3 &center, float radius)
    : center(center), radius(radius)
{
}

std::optional<RayHitInfo> Sphere::intersect(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 co = ray.origin - center;
    float a = glm::dot(ray.direction, ray.direction);
    float b = 2.f * glm::dot(co, ray.direction);
    float c = glm::dot(co, co) - radius * radius;

    float delta = b * b - 4.f * a * c;
    if (delta < 0)
    {
        return {};
    }

    float hit_t = (-b - sqrt(delta)) * 0.5f / a;
    if (hit_t < 0)
    {
        hit_t = (-b + sqrt(delta)) * 0.5f / a;
    }
    if (hit_t > t_min && hit_t < t_max)
    {
        glm::vec3 hit_point = ray.hit(hit_t);
        glm::vec3 normal = glm::normalize(hit_point - center);
        return RayHitInfo{hit_t, normal, hit_point};
    }

    return {};
}

Bound Sphere::getBound() const
{
    return Bound(center - glm::vec3(radius), center + glm::vec3(radius));
}