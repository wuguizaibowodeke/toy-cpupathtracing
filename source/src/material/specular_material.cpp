#include "material/specular_material.hpp"
#include "sampler/spherical.hpp"

SpecularMaterial::SpecularMaterial(const glm::vec3 &albedo)
    : m_albedo(albedo)
{
}

glm::vec3 SpecularMaterial::sampleBRDF(const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    beta *= m_albedo;
    return glm::vec3(-view_direction.x, view_direction.y, -view_direction.z); // 反射方向
}
