#pragma once

#include "camera/ray.hpp"
#include "accelerate/bound.hpp"
#include <optional>
#include <memory>

class Shape
{
public:
    virtual ~Shape() = default;

    virtual std::optional<RayHitInfo> intersect(
        const Ray &ray,
        float t_min = 1e-5,
        float t_max = std::numeric_limits<float>::infinity()) const = 0;
    
    virtual Bound getBound() const
    {
        return {};
    }
};

using ShapePtr = std::shared_ptr<Shape>;
