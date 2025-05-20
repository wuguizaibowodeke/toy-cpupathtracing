#include "accelerate/scene_bvh.hpp"
#include <array>
#include "util/debug_macro.hpp"
#include <iostream>
#include "log.hpp"

SceneBVH::SceneBVH(const std::vector<Instance> &instances)
{
}

SceneBVH::~SceneBVH()
{
}

std::optional<RayHitInfo> SceneBVH::intersect(const Ray &ray,
                                              float t_min,
                                              float t_max) const
{
    std::optional<RayHitInfo> closest_hitinfo;

    //临时存储交点实例
    const Instance *closest_instance = nullptr;

    DEBUG_LINE(size_t bounds_test_count = 0)

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

        if (node.instance_count == 0)
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
            auto instance_index = m_instances.begin() + node.instance_index;
            for (size_t i = 0; i < node.instance_count; ++i)
            {
                auto ray_object = ray.objectFromWorld(instance_index->object_from_world);
                auto hitinfo = instance_index->shape.intersect(ray_object, t_min, t_max);
                DEBUG_LINE(ray.bounds_test_count += ray_object.bounds_test_count)
                DEBUG_LINE(ray.triangle_test_count += ray_object.triangle_test_count)
                if (hitinfo.has_value())
                {
                    t_max = hitinfo->t;
                    closest_hitinfo = hitinfo;
                    closest_instance = &(*instance_index);
                }
                ++instance_index;
            }
            if (it == stack.begin())
            {
                break;
            }
            cuurent_index = *(--it);
        }
    }

    for(const auto &infinity_instance : m_infinite_instances)
    {
        auto ray_object = ray.objectFromWorld(infinity_instance.object_from_world);
        auto hitinfo = infinity_instance.shape.intersect(ray_object, t_min, t_max);
        DEBUG_LINE(ray.bounds_test_count += ray_object.bounds_test_count)
        DEBUG_LINE(ray.triangle_test_count += ray_object.triangle_test_count)
        if (hitinfo.has_value())
        {
            t_max = hitinfo->t;
            closest_hitinfo = hitinfo;
            closest_instance = &(infinity_instance);
        }       
    }

    if (closest_instance)
    {
        closest_hitinfo->hit_point = (closest_instance->world_from_object) * glm::vec4(closest_hitinfo->hit_point, 1.f);
        closest_hitinfo->normal = glm::normalize(glm::vec3(glm::transpose(closest_instance->object_from_world) * glm::vec4(closest_hitinfo->normal, 0.f)));
        closest_hitinfo->material = &closest_instance->material;
    }

#ifdef WITH_DEBUG_INFO
    ray.bounds_test_count = bounds_test_count;
#endif

    return closest_hitinfo;
}

Bound SceneBVH::getBound() const
{
    if (!m_nodes.empty())
    {
        return m_nodes[0].bound;
    }

    return {};
}

void SceneBVH::build(std::vector<Instance> &&instances)
{
    auto *root = m_node_allocator.allocate();
    auto temp_instances = std::move(instances);
    for(auto & instance : temp_instances)
    {
        if(instance.shape.getBound().isValid())
        {
            instance.updateBound();
            root->instances.push_back(instance);
        }
        else
        {
            m_infinite_instances.push_back(instance);
        }
    }
    root->updateBound();
    root->depth = 1;
    SceneBVHState state{};
    size_t instance_count = root->instances.size();
    recursiveSplit(root, state);

    LOG_D("Total Node Count: {}", state.total_node_count);
    LOG_D("Leaf Node Count: {}", state.leaf_node_count);
    LOG_D("Instance Count: {}", instance_count);
    LOG_D("Mean Leaf Node Instance Count: {}", static_cast<float>(instance_count) / static_cast<float>(state.leaf_node_count));
    LOG_D("Max Leaf Node Instance Count: {}", state.max_leaf_node_instance_count);
    LOG_D("Max Leaf Node Depth: {}", state.max_leaf_node_depth);

    m_nodes.reserve(state.total_node_count);
    m_instances.reserve(instance_count);
    flattenTree(root);
}

void SceneBVH::recursiveSplit(SceneBVHTreeNode *node, SceneBVHState &state)
{
    state.total_node_count++;
    if (node->instances.size() == 1 || node->depth > 32)
    {
        state.addLeafNode(node);
        return;
    }

    auto diag = node->bound.diagonal();
    size_t min_split_index = 0;
    Bound min_leftnode_bound{}, min_rightnode_bound{};
    size_t min_left_instance_count = 0, min_right_instance_count = 0;
    float min_cost = std::numeric_limits<float>::max();
    constexpr size_t bucket_count = 12;
    std::vector<size_t> instance_index_buckets[3][bucket_count] = {};
    for (size_t axis = 0; axis < 3; ++axis)
    {
        Bound buckets_bound[bucket_count] = {};
        size_t buckets_instance_count[bucket_count] = {};
        size_t instance_index = 0;
        for (const auto &instance : node->instances)
        {
            size_t bucket_index = glm::clamp<size_t>(
                glm::floor((instance.center[axis] - node->bound.getLeftBottom()[axis]) *
                           bucket_count / diag[axis]),
                0, bucket_count - 1);
            buckets_bound[bucket_index].expand(instance.bound);
            buckets_instance_count[bucket_index]++;
            instance_index_buckets[axis][bucket_index].push_back(instance_index);
            instance_index++;
        }

        Bound left_bound = buckets_bound[0];
        size_t left_instance_count = buckets_instance_count[0];
        for (size_t i = 1; i < bucket_count - 1; i++)
        {
            Bound right_bound{};
            size_t right_instance_count = 0;
            for (size_t j = bucket_count - 1; j >= i; j--)
            {
                right_bound.expand(buckets_bound[j]);
                right_instance_count += buckets_instance_count[j];
            }

            if (right_instance_count == 0)
            {
                break;
            }

            if (left_instance_count != 0)
            {
                float cost = left_bound.area() * left_instance_count +
                             right_bound.area() * right_instance_count;
                if (cost < min_cost)
                {
                    min_cost = cost;
                    min_split_index = i;
                    node->split_axis = axis;
                    min_leftnode_bound = left_bound;
                    min_rightnode_bound = right_bound;
                    min_left_instance_count = left_instance_count;
                    min_right_instance_count = right_instance_count;
                }
            }
            left_bound.expand(buckets_bound[i]);
            left_instance_count += buckets_instance_count[i];
        }
    }

    if (min_split_index == 0)
    {
        state.addLeafNode(node);
        return;
    }

    node->left = new SceneBVHTreeNode();
    node->left->depth = node->depth + 1;
    node->left->instances.reserve(min_left_instance_count);
    for (size_t i = 0; i < min_split_index; ++i)
    {
        for (size_t index : instance_index_buckets[node->split_axis][i])
        {
            node->left->instances.push_back(node->instances[index]);
        }
    }
    node->left->bound = min_leftnode_bound;

    node->right = new SceneBVHTreeNode();
    node->right->depth = node->depth + 1;
    node->right->instances.reserve(min_right_instance_count);
    for (size_t i = min_split_index; i < bucket_count; ++i)
    {
        for (size_t index : instance_index_buckets[node->split_axis][i])
        {
            node->right->instances.push_back(node->instances[index]);
        }
    }
    node->right->bound = min_rightnode_bound;

    node->instances.clear();
    node->instances.shrink_to_fit();

    recursiveSplit(node->left, state);
    recursiveSplit(node->right, state);
}

size_t SceneBVH::flattenTree(SceneBVHTreeNode *node)
{
    if (node == nullptr)
    {
        return 0;
    }

    SceneBVHNode SceneBVH_node{node->bound,
                               0,
                               static_cast<int>(node->instances.size()),
                               static_cast<uint8_t>(node->split_axis)};
    auto index = m_nodes.size();
    m_nodes.push_back(SceneBVH_node);
    if (SceneBVH_node.instance_count == 0)
    {
        flattenTree(node->left);
        m_nodes[index].child_index = flattenTree(node->right);
    }
    else
    {
        m_nodes[index].instance_index = m_instances.size();
        for (const auto &instance : node->instances)
        {
            m_instances.push_back(instance);
        }
    }

    return index;
}

SceneBVHTreeNodeAllocator::SceneBVHTreeNodeAllocator()
    : m_ptr(4096)
{
}

SceneBVHTreeNodeAllocator::~SceneBVHTreeNodeAllocator()
{
    for (auto &node : m_node_list)
    {
        delete[] node;
    }
    m_node_list.clear();
}

SceneBVHTreeNode *SceneBVHTreeNodeAllocator::allocate()
{
    if (m_ptr = 4096)
    {
        m_node_list.push_back(new SceneBVHTreeNode[4096]{});
        m_ptr = 0;
    }

    return &(m_node_list.back()[m_ptr++]);
}
