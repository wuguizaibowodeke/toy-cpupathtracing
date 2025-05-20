#pragma once

#include "camera/ray.hpp"
#include "shape.hpp"
#include <optional>

class Sphere : public Shape
{
public:
    Sphere(const glm::vec3 &center, float radius);

    ~Sphere() = default;

    glm::vec3 getCenter() const { return center; }

    float getRadius() const { return radius; }

    void setCenter(const glm::vec3 &c) { center = c; }

    void setRadius(float r) { radius = r; }

    virtual std::optional<RayHitInfo> intersect(
        const Ray &ray,
        float t_min = 1e-5,
        float t_max = std::numeric_limits<float>::infinity()) const override;

    virtual Bound getBound() const;
    
private:
    glm::vec3 center;

    float radius;
};
