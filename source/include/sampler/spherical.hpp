#pragma once 

#include <glm/glm.hpp>
#include "util/algorithm.hpp"

inline glm::vec2 uniformSampleDisk(const glm::vec2 &u)
{
    float r = std::sqrt(u.x);
    float theta = 2 * M_PI * u.y;

    return {r * glm::cos(theta), r * glm::sin(theta)};
}

inline glm::vec3 CosineSampleHemisphere(const glm::vec2 &u)
{
    float r = std::sqrt(u.x);
    float phi = 2 * M_PI * u.y;
    return {r * glm::cos(phi), glm::sqrt(1 - u.x), r * glm::sin(phi)};
}

inline glm::vec3 UniformSampleHemisphere(const RandomNumberGenerator &rng)
{
    glm::vec3 result;
    do
    {
        result = {rng.uniform(), rng.uniform(), rng.uniform()};
        result = result * 2.f - 1.f;
    } while (glm::length(result) > 1.0f);
    if (result.y < 0)
    {
        result.y = -result.y;
    }

    return glm::normalize(result);
}
