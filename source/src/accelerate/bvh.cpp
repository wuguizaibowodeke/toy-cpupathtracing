#include "accelerate/bvh.hpp"

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
    
    recursiveIntersect(ray, t_min, t_max, m_root, closest_hitinfo);

    return closest_hitinfo;    
}

void BVH::build(const std::vector<Triangle> &triangles)
{
    m_root = new BVHNode();
    m_root->triangles = std::move(triangles);
    m_root->updateBound();
    recursiveSplit(m_root);
}

void BVH::recursiveSplit(BVHNode *node)
{
    if (node->triangles.size() == 1)
    {
        return;
    }

    auto diag = node->bound.diagonal();
    size_t max_axis = diag.x > diag.y ? (diag.x > diag.z ? 0 : 2) : (diag.y > diag.z ? 1 : 2);
    float mid = node->bound.getLeftBottom()[max_axis] + diag[max_axis] / 2;
    std::vector<Triangle> left_triangles, right_triangles;
    for (const auto &triangle : node->triangles)
    {
        if (triangle.getVertex(0)[max_axis] < mid)
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

    node->left = new BVHNode();
    node->left->triangles.clear();
    node->left->triangles.shrink_to_fit();
    node->left->triangles = std::move(left_triangles);
    node->left->updateBound();

    node->right = new BVHNode();
    node->right->triangles.clear();
    node->right->triangles.shrink_to_fit();
    node->right->triangles = std::move(right_triangles);
    node->right->updateBound();

    recursiveSplit(node->left);
    recursiveSplit(node->right);
}

void BVH::recursiveIntersect(const Ray &ray, float t_min, float t_max, BVHNode *node, std::optional<RayHitInfo> &closest_hitinfo) const
{
    if (node == nullptr)
    {
        return;
    }

    if (!node->bound.isIntersect(ray, t_min, t_max))
    {
        return;
    }

    if (node->triangles.empty())
    {
        recursiveIntersect(ray, t_min, t_max, node->left, closest_hitinfo);
        recursiveIntersect(ray, t_min, t_max, node->right, closest_hitinfo);
    }
    else
    {
        for (const auto &triangle : node->triangles)
        {
            auto hitinfo = triangle.intersect(ray, t_min, t_max);
            if (hitinfo.has_value())
            {
                t_max = hitinfo->t;
                closest_hitinfo = hitinfo;
            }
        }        
    }
}
