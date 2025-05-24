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

    glm::bvec3 dir_is_neg = {
        ray.direction.x < 0,
        ray.direction.y < 0,
        ray.direction.z < 0};

    glm::vec3 inv_dir = 1.0f / ray.direction;

    std::array<int, 32> stack;
    auto it = stack.begin();
    size_t cuurent_index = 0;

    while (true)
    {
        auto &node = m_nodes[cuurent_index];
        DEBUG_LINE(bounds_test_count++;)
        if (!node.bound.isIntersect(ray, inv_dir, t_min, t_max))
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
            if (dir_is_neg[node.split_axis])
            {
                *(it++) = cuurent_index + 1;
                cuurent_index = node.child_index;
            }
            else
            {
                cuurent_index++;
                *(it++) = node.child_index;
            }
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
    ray.bounds_test_count = bounds_test_count;
    ray.triangle_test_count = triangle_test_count;
#endif

    return closest_hitinfo;
}

Bound BVH::getBound() const
{
    if (!m_nodes.empty())
    {
        return m_nodes[0].bound;
    }

    return {};
}

void BVH::build(const std::vector<Triangle> &triangles)
{
    auto* root = m_node_allocator.allocate();
    root->triangles = std::move(triangles);
    root->updateBound();
    root->depth = 1;
    BVHState state{};
    size_t triangle_count = root->triangles.size();
    recursiveSplit(root, state);

    LOG_D("Total Node Count: {}", state.total_node_count);
    LOG_D("Leaf Node Count: {}", state.leaf_node_count);
    LOG_D("Triangle Count: {}", triangle_count);
    LOG_D("Mean Leaf Node Triangle Count: {}", static_cast<float>(triangle_count) / static_cast<float>(state.leaf_node_count));
    LOG_D("Max Leaf Node Triangle Count: {}", state.max_leaf_node_triangle_count);
    LOG_D("Max Leaf Node Depth: {}", state.max_leaf_node_depth);

    m_nodes.reserve(state.total_node_count);
    m_triangles.reserve(triangle_count);
    flattenTree(root);
}

void BVH::recursiveSplit(BVHTreeNode *node, BVHState &state)
{
    state.total_node_count++;
    if (node->triangles.size() == 1 || node->depth > 32)
    {
        state.addLeafNode(node);
        return;
    }

    auto diag = node->bound.diagonal();
    size_t min_split_index = 0;
    Bound min_leftnode_bound{}, min_rightnode_bound{};
    size_t min_left_triangle_count = 0, min_right_triangle_count = 0;
    float min_cost = std::numeric_limits<float>::max();
    constexpr size_t bucket_count = 12;
    std::vector<size_t> triangle_index_buckets[3][bucket_count] = {};
    for(size_t axis = 0; axis < 3; ++axis)
    {
        Bound buckets_bound[bucket_count] = {};
        size_t buckets_triangle_count[bucket_count] = {};
        size_t triangle_index = 0;
        for(const auto &triangle : node->triangles)
        {
            auto traiangle_center = (triangle.getVertex(0)[axis] +
                                    triangle.getVertex(1)[axis]  +
                                    triangle.getVertex(2)[axis] ) /
                                   3.0f;
            size_t bucket_index  = glm::clamp<size_t>(
                glm::floor((traiangle_center - node->bound.getLeftBottom()[axis]) * 
                           bucket_count / diag[axis]), 0, bucket_count - 1);
            buckets_bound[bucket_index].expand(triangle.getVertex(0));
            buckets_bound[bucket_index].expand(triangle.getVertex(1));
            buckets_bound[bucket_index].expand(triangle.getVertex(2));
            buckets_triangle_count[bucket_index]++;
            triangle_index_buckets[axis][bucket_index].push_back(triangle_index);
            triangle_index++;
        }

        Bound left_bound = buckets_bound[0];
        size_t left_triangle_count = buckets_triangle_count[0];
        for (size_t i = 1; i < bucket_count -1 ; i++)
        {
            Bound right_bound{};
            size_t right_triangle_count = 0;
            for (size_t j = bucket_count - 1; j >= i; j--)
            {
                right_bound.expand(buckets_bound[j]);
                right_triangle_count += buckets_triangle_count[j];
            }

            if(right_triangle_count == 0)
            {
                break;
            }

            if(left_triangle_count != 0)
            {
                float cost = left_bound.area() * left_triangle_count +
                             right_bound.area() * right_triangle_count;
                if (cost < min_cost)
                {
                    min_cost = cost;
                    min_split_index = i;
                    node->split_axis = axis;
                    min_leftnode_bound = left_bound;
                    min_rightnode_bound = right_bound;
                    min_left_triangle_count = left_triangle_count;
                    min_right_triangle_count = right_triangle_count;
                }
            }
            left_bound.expand(buckets_bound[i]);
            left_triangle_count += buckets_triangle_count[i];
        }     
    }

    if(min_split_index == 0)
    {
        state.addLeafNode(node);
        return;
    }

    node->left = m_node_allocator.allocate();
    node->left->depth = node->depth + 1;
    node->left->triangles.reserve(min_left_triangle_count);
    for(size_t i = 0; i < min_split_index; ++i)
    {
        for (size_t index: triangle_index_buckets[node->split_axis][i])
        {
            node->left->triangles.push_back(node->triangles[index]);
        }
    }
    node->left->bound = min_leftnode_bound;

    node->right = m_node_allocator.allocate();
    node->right->depth = node->depth + 1;
    node->right->triangles.reserve(min_right_triangle_count);
    for(size_t i = min_split_index; i < bucket_count; ++i)
    {
        for (size_t index: triangle_index_buckets[node->split_axis][i])
        {
            node->right->triangles.push_back(node->triangles[index]);
        }
    }
    node->right->bound = min_rightnode_bound;

    node->triangles.clear();
    node->triangles.shrink_to_fit();

    recursiveSplit(node->left, state);
    recursiveSplit(node->right, state);
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
                     static_cast<uint8_t>(node->split_axis)};
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

BVHTreeNodeAllocator::BVHTreeNodeAllocator()
    :m_ptr(4096)
{
}

BVHTreeNodeAllocator::~BVHTreeNodeAllocator()
{
    for (auto &node : m_node_list)
    {
        delete[] node;
    }
    m_node_list.clear();
}

BVHTreeNode *BVHTreeNodeAllocator::allocate()
{
    if(m_ptr == 4096)
    {
        m_node_list.push_back(new BVHTreeNode[4096]{});
        m_ptr = 0;
    }

    return &(m_node_list.back()[m_ptr++]);
}
