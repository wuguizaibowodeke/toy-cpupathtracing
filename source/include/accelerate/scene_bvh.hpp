#pragma once

#include "bound.hpp"
#include "geometry/shape.hpp"

struct Instance
{
    ShapePtr shape;
    MaterialPtr material;
    glm::mat4 world_from_object;
    glm::mat4 object_from_world;
    //世界坐标系下的包围盒
    Bound bound {};
    glm::vec3 center {};

    void updateBound()
    {
        bound = {};
        auto bounds_object = shape->getBound();
        for(size_t i = 0; i < 8; i++)
        {
            auto corner = bounds_object.getCorner(i);
            glm::vec4 world_corner = world_from_object * glm::vec4(corner, 1.0f);
            bound.expand(glm::vec3(world_corner));
        }
        center = (bound.getLeftBottom() + bound.getRightTop()) * 0.5f;
    }
};

struct SceneBVHTreeNode
{
    Bound bound{};
    std::vector<Instance> instances{};
    SceneBVHTreeNode *left = nullptr;
    SceneBVHTreeNode *right = nullptr;
    size_t depth;
    size_t split_axis;

    void updateBound()
    {
        bound = {};
        for (const auto &instance : instances)
        {
            bound.expand(instance.bound);
        }
    }
};

struct SceneBVHNode
{
    Bound bound{};
    union
    {
        int child_index;
        int instance_index;
    };
    int instance_count;
    uint8_t split_axis;
};

struct SceneBVHState
{
    size_t total_node_count{};
    size_t leaf_node_count{};
    size_t max_leaf_node_instance_count{};
    size_t max_leaf_node_depth{};

    void addLeafNode(SceneBVHTreeNode *node)
    {
        leaf_node_count++;
        max_leaf_node_instance_count = glm::max(max_leaf_node_instance_count, node->instances.size());
        max_leaf_node_depth = glm::max(max_leaf_node_depth, node->depth);
    }
};

class SceneBVHTreeNodeAllocator
{
public:
    SceneBVHTreeNodeAllocator();

    ~SceneBVHTreeNodeAllocator();

    SceneBVHTreeNode *allocate();
private:
    size_t m_ptr;
    std::vector<SceneBVHTreeNode *> m_node_list;
};


class SceneBVH : public Shape
{
public:
    SceneBVH() = default;

    SceneBVH(const std::vector<Instance> &instances);

    ~SceneBVH();

    virtual std::optional<RayHitInfo> intersect(const Ray &ray,
                                                float t_min,
                                                float t_max) const override;

    virtual Bound getBound() const override;

    void build(std::vector<Instance> &&Instances);
    
private:
    void recursiveSplit(SceneBVHTreeNode *node, SceneBVHState& state);

    size_t flattenTree(SceneBVHTreeNode *node);

private:
    SceneBVHTreeNodeAllocator m_node_allocator{};
    std::vector<SceneBVHNode> m_nodes;
    //包围盒有限大
    std::vector<Instance> m_instances;
    //包围盒无限大
    std::vector<Instance> m_infinite_instances;
};
