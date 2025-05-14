#pragma once

#include "geometry/triangle.hpp"
#include <filesystem>
#include "accelerate/bvh.hpp"

class Model : public Shape
{
public:
    Model(const std::vector<Triangle> &triangles);

    Model(const std::filesystem::path &filename);

    ~Model() = default;

    std::optional<RayHitInfo> intersect(const Ray &ray, float t_min,
                                        float t_max) const override;
                          
private:
    BVH m_bvh{};
};