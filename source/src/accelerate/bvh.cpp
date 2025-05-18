#include "accelerate/bvh.hpp"
#include <array>
#include "util/debug_macro.hpp"
#include <iostream>
#include "log.hpp"

BVH::BVH(const std::vector<Triangle> &triangles)
{
    build(triangles);
}

BVH::~BVH()
{
}

std::optional<RayHitInfo> BVH::intersect(const Ray &ray,
                                         float t_min,
                                         float t_max) const
{
    std::optional<RayHitInfo> closest_hitinfo;

    DEBUG_LINE(size_t bounds_test_count = 0)
    DEBUG_LINE(size_t triangle_test_count = 0)

    std::array<int, 32> stack;
    auto it = stack.begin();
    size_t cuurent_index = 0;

    while (true)
    {
        auto &node = m_nodes[cuurent_index];
        DEBUG_LINE(bounds_test_count++;)
        if (!node.bound.isIntersect(ray, t_min, t_max))
        {
            if (it == stack.begin())
            {
                break;
            }
            cuurent_index = *(--it);
            continue;
        }

        if (node.triangle_count == 0)
        {
            cuurent_index++;
            *(it++) = node.child_index;
        }
        else
        {
            auto triangle_index = m_triangles.begin() + node.triangle_index;
            DEBUG_LINE(triangle_test_count += node.triangle_count)
            for (size_t i = 0; i < node.triangle_count; ++i)
            {
                auto hitinfo = triangle_index->intersect(ray, t_min, t_max);
                ++triangle_index;
                if (hitinfo.has_value())
                {
                    t_max = hitinfo->t;
                    closest_hitinfo = hitinfo;
                    DEBUG_LINE(closest_hitinfo->bounds_depth = node.depth)
                }
            }
            if (it == stack.begin())
            {
                break;
            }
            cuurent_index = *(--it);
        }
    }

#ifdef WITH_DEBUG_INFO
    if (closest_hitinfo.has_value())
    {
        closest_hitinfo->bounds_test_count = bounds_test_count;
        closest_hitinfo->triangle_test_count = triangle_test_count;
    }
#endif

    return closest_hitinfo;
}

void BVH::build(const std::vector<Triangle> &triangles)
{
    auto root = new BVHTreeNode();
    root->triangles = std::move(triangles);
    root->updateBound();
    root->depth = 1;
    recursiveSplit(root);
    flattenTree(root);
    delete root;
}

void BVH::recursiveSplit(BVHTreeNode *node)
{
    if (node->triangles.size() == 1 || node->depth > 32)
    {
        return;
    }

    auto diag = node->bound.diagonal();
    size_t max_axis = diag.x > diag.y ? (diag.x > diag.z ? 0 : 2) : (diag.y > diag.z ? 1 : 2);
    float mid = node->bound.getLeftBottom()[max_axis] + diag[max_axis] / 2;
    std::vector<Triangle> left_triangles, right_triangles;
    for (const auto &triangle : node->triangles)
    {
        auto average = (triangle.getVertex(0)[max_axis] +
                        triangle.getVertex(1)[max_axis] +
                        triangle.getVertex(2)[max_axis]) /
                       3.0f;
        if (average < mid)
        {
            left_triangles.push_back(triangle);
        }
        else
        {
            right_triangles.push_back(triangle);
        }
    }

    if (left_triangles.empty() || right_triangles.empty())
    {
        return;
    }

    node->triangles.clear();
    node->triangles.shrink_to_fit();
    
    node->left = new BVHTreeNode();
    node->left->triangles = std::move(left_triangles);
    node->left->updateBound();
    node->left->depth = node->depth + 1;

    node->right = new BVHTreeNode();
    node->right->triangles = std::move(right_triangles);
    node->right->updateBound();
    node->right->depth = node->depth + 1;

    recursiveSplit(node->left);
    recursiveSplit(node->right);
}

size_t BVH::flattenTree(BVHTreeNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    BVHNode bvh_node{node->bound,
                     0,
                     static_cast<int>(node->triangles.size()),
                     static_cast<uint8_t>(node->depth)};
    auto index = m_nodes.size();
    m_nodes.push_back(bvh_node);
    if (bvh_node.triangle_count == 0)
    {
        flattenTree(node->left);
        m_nodes[index].child_index = flattenTree(node->right);
    }
    else
    {
        m_nodes[index].triangle_index = m_triangles.size();
        for (const auto &triangle : node->triangles)
        {
            m_triangles.push_back(triangle);
        }
    }

    return index;
}
