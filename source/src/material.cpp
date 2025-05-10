#include "material.hpp"

Material::Material()
    : m_albedo(1.0f, 1.0f, 1.0f), m_emissive(1.0f, 1.0f, 1.0f), m_is_specular(32.0f)
{
}

Material::Material(const glm::vec3 &albedo, const glm::vec3 &emissive, bool is_specular)
    : m_albedo(albedo), m_emissive(emissive), m_is_specular(is_specular)
{
}
