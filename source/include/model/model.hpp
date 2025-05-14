#pragma once

#include "geometry/triangle.hpp"
#include <filesystem>
#include "accelerate/bound.hpp"

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> &triangles);

    Model(const std::filesystem::path &filename);

    ~Model() = default;

    std::optional<RayHitInfo> intersect(const Ray &ray, float t_min,
                                        float t_max) const override;

private:
    void calculateBound();                                       

private:
    std::vector<Triangle> m_triangles;
    Bound m_bound;
};