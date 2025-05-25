#include "material/grid_ground_material.hpp"
#include "sampler/spherical.hpp"

GridGroundMaterial::GridGroundMaterial(const glm::vec3 &albedo)
    : m_albedo(albedo)
{
    m_enable_refraction = true;
}

glm::vec3 GridGroundMaterial::sampleBRDF(const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    return glm::vec3();
}

glm::vec3 GridGroundMaterial::sampleBSDF(const glm::vec3 &hit_point, const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    beta *= m_albedo;
    if ((static_cast<int>(glm::floor(hit_point.x * 8)) % 8 == 0) ||
        (static_cast<int>(glm::floor(hit_point.z * 8)) % 8 == 0))
    {
        beta *= 0.1f;
    }

    return CosineSampleHemisphere(glm::vec2(rng.uniform(), rng.uniform()));
}
