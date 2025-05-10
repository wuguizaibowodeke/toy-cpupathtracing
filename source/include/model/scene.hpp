#pragma once

#include "geometry/shape.hpp"
#include "model/material.hpp"

class Scene : public Shape
{
    struct Instance
    {
        const Shape &shape;
        Material material;
        glm::mat4 world_from_object;
        glm::mat4 object_from_world;
    };

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

private:
    std::vector<Instance> m_instances;
};