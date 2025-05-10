#pragma once
#include <glm/glm.hpp>
#include "material.hpp"

struct Ray {
    glm::vec3 origin;
    glm::vec3 direction;

    glm::vec3 hit(float t) const { return origin + t * direction; }

    Ray objectFromWorld(const glm::mat4 &object_from_world) const;
};

struct RayHitInfo {
    float t;
    glm::vec3 normal;
    glm::vec3 hit_point;
    const Material *material = nullptr;
};

