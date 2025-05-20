#pragma once

#include "model/material.hpp"
#include "accelerate/scene_bvh.hpp"
#include <vector>

class Scene : public Shape
{
public:
    Scene() = default;

    virtual ~Scene();

    void addInstance(const Shape &shape,
                     const Material &material = {},
                     const glm::vec3 &pos = {0, 0, 0},
                     const glm::vec3 &scale = {1, 1, 1},
                     const glm::vec3 &rotate = {0, 0, 0});

    virtual std::optional<RayHitInfo> intersect(const Ray &ray,
                                                float t_min = 1e-5,
                                                float t_max = std::numeric_limits<float>::infinity()) const override;

    void buildBVH();
private:
    std::vector<Instance> m_instances;

    SceneBVH m_bvh{};  
};