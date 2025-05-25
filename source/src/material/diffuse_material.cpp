#include "material/diffuse_material.hpp"
#include "sampler/spherical.hpp"

DiffuseMaterial::DiffuseMaterial(const glm::vec3 &albedo)
    : m_albedo(albedo)
{
}

glm::vec3 DiffuseMaterial::sampleBRDF(const glm::vec3 &view_direction, glm::vec3 &beta, const RandomNumberGenerator &rng) const
{
    beta *= m_albedo;
    return CosineSampleHemisphere(glm::vec2(rng.uniform(), rng.uniform()));
}
