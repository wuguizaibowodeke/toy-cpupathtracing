#pragma once

#include "bound.hpp"
#include "geometry/triangle.hpp"

struct BVHNode
{
    Bound bound{};
    std::vector<Triangle> triangles{};
    BVHNode *left = nullptr;
    BVHNode *right = nullptr;

    void updateBound()
    {
        bound = {};
        for (const auto &triangle : triangles)
        {
            bound.expand(triangle.getVertex(0));
            bound.expand(triangle.getVertex(1));
            bound.expand(triangle.getVertex(2));
        }
    }
};

class BVH : public Shape
{
public:
    BVH() = default;

    BVH(const std::vector<Triangle> &triangles);

    ~BVH();

    virtual std::optional<RayHitInfo> intersect(const Ray &ray,
                                                float t_min,
                                                float t_max) const override;

private:
    void build(const std::vector<Triangle> &triangles);

    void recursiveSplit(BVHNode *node);

    void recursiveIntersect(const Ray &ray,
                                float t_min,
                                float t_max,
                                BVHNode *node,
                                std::optional<RayHitInfo> &closest_hitinfo) const;

private:
    BVHNode *m_root;
};
