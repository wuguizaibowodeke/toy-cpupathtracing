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
    size_t split_axis;

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

struct /*alignas(32)*/ BVHNode
{
    Bound bound{};
    union
    {
        int child_index;
        int triangle_index;
    };
    int triangle_count;
    uint8_t depth;
    uint8_t split_axis;
};

struct BVHState
{
    size_t total_node_count{};
    size_t leaf_node_count{};
    size_t max_leaf_node_triangle_count{};

    void addLeafNode(BVHTreeNode *node)
    {
        leaf_node_count++;
        max_leaf_node_triangle_count = glm::max(max_leaf_node_triangle_count, node->triangles.size());
    }
};

class BVHTreeNodeAllocator
{
public:
    BVHTreeNodeAllocator();

    ~BVHTreeNodeAllocator();

    BVHTreeNode *allocate();
private:
    size_t m_ptr;
    std::vector<BVHTreeNode *> m_node_list;
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

    void recursiveSplit(BVHTreeNode *node, BVHState& state);

    size_t flattenTree(BVHTreeNode *node);

private:
    BVHTreeNodeAllocator m_node_allocator{};
    std::vector<BVHNode> m_nodes;
    std::vector<Triangle> m_triangles;
};
