#pragma once

#include "bound.hpp"
#include "geometry/triangle.hpp"

struct BVHTreeNode
{
    Bound bound{};
    std::vector<Triangle> triangles{};
    BVHTreeNode *left = nullptr;
    BVHTreeNode *right = nullptr;
    size_t depth;

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

struct alignas(32) BVHNode
{
    Bound bound{};
    union
    {
        int child_index;
        int triangle_index;
    };   
    uint16_t triangle_count;
    uint8_t depth;
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

    void recursiveSplit(BVHTreeNode *node);

    size_t flattenTree(BVHTreeNode *node);

private:
    std::vector<BVHNode> m_nodes;
    std::vector<Triangle> m_triangles;
};
