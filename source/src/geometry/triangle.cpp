#include "geometry/triangle.hpp"
#include <iostream>

Triangle::Triangle(const glm::vec3 &v0, const glm::vec3 &v1, const glm::vec3 &v2,
                   const glm::vec3 &normal0, const glm::vec3 &normal1, const glm::vec3 &normal2)
    : v0(v0), v1(v1), v2(v2),
      normal0(normal0), normal1(normal1), normal2(normal2)
{
}

Triangle::Triangle(const glm::vec3 v0, const glm::vec3 v1, const glm::vec3 v2)
    : v0(v0), v1(v1), v2(v2)
{
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 normal = glm::normalize(glm::cross(edge1, edge2));
    normal0 = normal;
    normal1 = normal;
    normal2 = normal;
}

std::optional<RayHitInfo> Triangle::intersect(const Ray &ray, float t_min, float t_max) const
{
    glm::vec3 edge1 = v1 - v0;
    glm::vec3 edge2 = v2 - v0;
    glm::vec3 s1 = glm::cross(ray.direction, edge2);
    float inv_det = 1.0f / glm::dot(s1, edge1);

    glm::vec3 s = ray.origin - v0;
    float u = glm::dot(s1, s) * inv_det;
    if (u < 0 || u > 1)
    {
        return {};
    }

    glm::vec3 s2 = glm::cross(s, edge1);
    float v = glm::dot(s2, ray.direction) * inv_det;
    if (v < 0 || u + v > 1)
    {
        return {};
    }

    float t = glm::dot(s2, edge2) * inv_det;
    if (t >= t_min && t <= t_max)
    {
        glm::vec3 hit_point = ray.hit(t);
        glm::vec3 normal = (1.f - u - v) * normal0 + u * normal1 + v * normal2;
        return RayHitInfo{t, glm::normalize(normal), hit_point};
    }

    return {};
}
glm::vec3 Triangle::getVertex(int index) const
{
    if(index > 2 || index < 0)
    {
        std::cerr << "Triangle::getVertex: index out of range" << std::endl;
        return glm::vec3();
    }

    if (index == 0)
    {
        return v0;
    }
    else if (index == 1)
    {
        return v1;
    }
    else if (index == 2)
    {
        return v2;
    }

    return glm::vec3();
}

glm::vec3 Triangle::getNormal(int index) const
{
    if(index > 2 || index < 0)
    {
        std::cerr << "Triangle::getNormal: index out of range" << std::endl;
        return glm::vec3();
    }

    if (index == 0)
    {
        return normal0;
    }
    else if (index == 1)
    {
        return normal1;
    }
    else if (index == 2)
    {
        return normal2;
    }

    return glm::vec3();
}
