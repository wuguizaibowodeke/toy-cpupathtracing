#pragma once
#include <glm/glm.hpp>
#include "model/material.hpp"

struct Ray
{
    glm::vec3 origin;

    glm::vec3 direction;

    glm::vec3 hit(float t) const { return origin + t * direction; }

    Ray objectFromWorld(const glm::mat4 &object_from_world) const;

#ifdef WITH_DEBUG_INFO
    mutable size_t bounds_test_count = 0;
    mutable size_t triangle_test_count = 0;
#endif
};

struct RayHitInfo
{
    float t;
    glm::vec3 normal;
    glm::vec3 hit_point;
    MaterialPtr material = nullptr;
};
