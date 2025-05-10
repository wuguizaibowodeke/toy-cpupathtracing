#pragma once

#include "geometry/shape.hpp"

class Plane : public Shape
{
public:
    explicit Plane(const glm::vec3 &point, const glm::vec3 &normal);

    ~Plane() = default;

    virtual std::optional<RayHitInfo> intersect(
        const Ray &ray,
        float t_min = 1e-5,
        float t_max = std::numeric_limits<float>::infinity()) const override;

private:
    glm::vec3 m_normal;
    glm::vec3 m_point;
};