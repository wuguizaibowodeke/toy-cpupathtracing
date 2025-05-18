#pragma once
#include <glm/glm.hpp>
#include "model/material.hpp"

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 hit(float t) const { return origin + t * direction; }

    Ray objectFromWorld(const glm::mat4 &object_from_world) const;
};

struct RayHitInfo
{
    float t;
    glm::vec3 normal;
    glm::vec3 hit_point;
    const Material *material = nullptr;

#ifdef WITH_DEBUG_INFO
    size_t bounds_test_count = 0;
    size_t triangle_test_count = 0;
    size_t bounds_depth = 0;
#endif
};
